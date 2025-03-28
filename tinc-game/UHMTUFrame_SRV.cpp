#include "boost/process.hpp"
#include <boost/process/windows.hpp>
#include "UHMTUFrame.h"
#include <sstream>
#include <thread>

struct MeasureMtuResult {
	enum Enum {
		MeasureMTU_NoResult,
		MeasureMTU_Pass,
		MeasureMTU_DF
	};
};

ReturnValue<CheckAddressFormatResult::Enum> UHMTUFrame::API_SRV_CheckAddressFormat(std::wstring ipAddress)
{
	ReturnValue<CheckAddressFormatResult::Enum> result(
		false,
		CheckAddressFormatResult::CheckAddressFormat_Other
	);

	{
		const int POSIX_INET6_ADDRSTRLEN = 46;
		if (ipAddress.length() > POSIX_INET6_ADDRSTRLEN) {
			return false;
		}
	}

	{
		// Check IPv6 MTU may only supported on Linux with iputils ping
		bool validIpv4 = true;

		const auto delimiter_ipv4 = L'.';
		if (validIpv4) {
			int dotCout = std::count(ipAddress.begin(), ipAddress.end(), delimiter_ipv4);
			if (dotCout != 3) {
				validIpv4 = false;
			}
		}

		if (validIpv4) {
			std::wstringstream ipStream(ipAddress);
			std::wstring tempDelimited;
			int streamIndex = 0;
			while (std::getline(ipStream, tempDelimited, delimiter_ipv4)) {
				if (tempDelimited.length() > 3) {
					validIpv4 = false;
					break;
				}
				for (int tempIndex = 0; tempIndex < tempDelimited.length(); tempIndex++) {
					bool isDigit = std::isdigit(tempDelimited[tempIndex]);
					if (isDigit == false) {
						validIpv4 = false;
						break;
					}
				}
				if (validIpv4 == false) {
					break;
				}

				int ipOctet = std::stoi(tempDelimited);
				bool validOctet = ipOctet > 0 && ipOctet < 256;
				if (validOctet == false) {
					validIpv4 = false;
					break;
				}

				streamIndex++;
				if (streamIndex >= 4) {
					break;
				}
			}
		}

		if (validIpv4) {
			result.success = true;
			return result;
		}
	}
}

std::wstring ParseLocalStringToWstring(std::string input) {
	return std::wstring(input.begin(), input.end());
}

MeasureMtuResult::Enum MeasureMTU(std::wstring ipAddress, int mtu, UHMTUFrame* frame) {
	const int PING_OVERHEAD = 28;
	auto pingMtu = mtu - PING_OVERHEAD;
	auto result = MeasureMtuResult::MeasureMTU_NoResult;

	namespace bp = boost::process;
	bp::ipstream is;

	bp::child c(L"ping437.bat", ipAddress, std::to_string(pingMtu), bp::std_out > is, bp::windows::hide);

	std::string line;

	while (std::getline(is, line)) {
		// "Reply from <ip address>: bytes=..."
		if (line.find(std::string(": b")) != std::string::npos) {
			auto wline = ParseLocalStringToWstring(line);
			frame->CallAfter(&UHMTUFrame::API_UI_ReportStatus, wline);
			result = MeasureMtuResult::MeasureMTU_Pass;
			continue;
		}
		// "Request timed out." || "Reply from <ip address>: Destination host unreachable."
		if (line.length() == 19 || line.find(std::string(": D")) != std::string::npos) {
			auto wline = ParseLocalStringToWstring(line);
			frame->CallAfter(&UHMTUFrame::API_UI_ReportStatus, wline);
			continue;
		}
		// "Packet needs to be fragmented but DF set."
		if (line.length() == 42 && line[34] == 'D' && line[35] == 'F') {
			auto wline = ParseLocalStringToWstring(line);
			frame->CallAfter(&UHMTUFrame::API_UI_ReportStatus, wline);
			result = MeasureMtuResult::MeasureMTU_DF;
			continue;
		}
	}

	c.wait();
	return result;
}

/// <summary>
/// Effective range is 576 ~ 1500, other range may work but undefined behavior
/// </summary>
/// <param name="min"></param>
/// <param name="max"></param>
/// <returns></returns>
int MtuHalfSearch(std::wstring ipAddress, UHMTUFrame* frame, int min = 576, int max = 1500) {
	// This algorithm can't properly handle mid == max
	// Set a delicated upper boundary to let max fall into range between min and upper boundary
	int upperBoundary = max + 1;

	int failedCount = 0;
	int recentSuccessValue = 0;
	bool stopNextLoop = false;
	while (stopNextLoop == false) {
		int diff = upperBoundary - min;

		bool divisible = diff > 1;
		if (divisible == false) {
			// break now or perform do-while this time?
			if (min == recentSuccessValue) {
#if VERBOSE == 1
				if (verbose) {
					std::cout << "Min: " << min << " UpperBoundary: " << upperBoundary << " Diff: " << diff << " Min == RecentSuccessValue" << std::endl;
				}
#endif
				break;
	}
			stopNextLoop = true;
		}

		int halfDiff = diff / 2;

#if VERBOSE == 1
		if (verbose) {
			std::cout << "Min: " << min << " UpperBoundary: " << upperBoundary << " Diff: " << diff;
		}
#endif

		int mid = min + halfDiff;
#if VERBOSE == 1
		if (verbose) {
			std::cout << " Mid: " << mid;
		}
#endif
		auto testMidResult = MeasureMTU(ipAddress, mid, frame);

		if (testMidResult == MeasureMtuResult::MeasureMTU_Pass) {
			recentSuccessValue = mid;
			frame->API_UI_ReportMTU_IPv4(recentSuccessValue);
#if VERBOSE == 1
			if (verbose) {
				std::cout << " TestMid: true" << std::endl;
			}
#endif
			min = min + halfDiff;
		}
		else if (testMidResult == MeasureMtuResult::MeasureMTU_DF) {
#if VERBOSE == 1
			if (verbose) {
				std::cout << " TestMid: false" << std::endl;
			}
#endif
			upperBoundary = upperBoundary - halfDiff;
		}
		else if (testMidResult == MeasureMtuResult::MeasureMTU_NoResult) {
			failedCount++;
			if (failedCount > 2) {
				return 0;
			}
		}

		const auto PING_DELAY_PREVENT_ABUSE = std::chrono::milliseconds(500);
		std::this_thread::sleep_for(PING_DELAY_PREVENT_ABUSE);
	}
#if VERBOSE == 1
	if (verbose) {
		std::cout << "Result: " << recentSuccessValue << std::endl;
	}
#endif

	return recentSuccessValue;
}

void UHMTUFrame::API_SRV_StartMeasureMTU(std::wstring ipAddress)
{
	auto mtuResult = MtuHalfSearch(ipAddress, this);

	if (mtuResult != 0) {
		const int IPV6_OVERHEAD = 20;
		CallAfter(&UHMTUFrame::API_UI_ReportMTU_IPv6, mtuResult - IPV6_OVERHEAD);
		CallAfter(&UHMTUFrame::API_UI_EndMeasureMTU, true, L"");
	}
	else {
		CallAfter(&UHMTUFrame::API_UI_EndMeasureMTU, false, L"Check your connection or use another IP address\r");
	}
}
