#include "UHMTUFrame.h"

void UHMTUFrameTest::TestCheckAddressFormat(wxWindow* parent) {
	auto testFailedMsg = std::wstring(L"Test failed: ");

	auto testSample = std::wstring(L"1.1.1.1");

	auto testResult = UHMTUFrame::API_SRV_CheckAddressFormat(testSample);
	if (testResult == false) {
		wxMessageDialog(parent, testFailedMsg + testSample).ShowModal();
	}
	testSample = L"11.1.1.1";
	testResult = UHMTUFrame::API_SRV_CheckAddressFormat(testSample);
	if (testResult == false) {
		wxMessageDialog(parent, testFailedMsg + testSample).ShowModal();
	}
	testSample = L"111.1.1.1";
	testResult = UHMTUFrame::API_SRV_CheckAddressFormat(testSample);
	if (testResult == false) {
		wxMessageDialog(parent, testFailedMsg + testSample).ShowModal();
	}
	testSample = L"1111.1.1.1";
	testResult = UHMTUFrame::API_SRV_CheckAddressFormat(testSample);
	if (testResult == true) {
		wxMessageDialog(parent, testFailedMsg + testSample).ShowModal();
	}
	testSample = L"1.1.1.1";
	testResult = UHMTUFrame::API_SRV_CheckAddressFormat(testSample);
	if (testResult == false) {
		wxMessageDialog(parent, testFailedMsg + testSample).ShowModal();
	}
	testSample = L"1.11.1.1";
	testResult = UHMTUFrame::API_SRV_CheckAddressFormat(testSample);
	if (testResult == false) {
		wxMessageDialog(parent, testFailedMsg + testSample).ShowModal();
	}
	testSample = L"1.111.1.1";
	testResult = UHMTUFrame::API_SRV_CheckAddressFormat(testSample);
	if (testResult == false) {
		wxMessageDialog(parent, testFailedMsg + testSample).ShowModal();
	}
	testSample = L"1.1111.1.1";
	testResult = UHMTUFrame::API_SRV_CheckAddressFormat(testSample);
	if (testResult == true) {
		wxMessageDialog(parent, testFailedMsg + testSample).ShowModal();
	}
	testSample = L"1.1.1.1";
	testResult = UHMTUFrame::API_SRV_CheckAddressFormat(testSample);
	if (testResult == false) {
		wxMessageDialog(parent, testFailedMsg + testSample).ShowModal();
	}
	testSample = L"1.1.11.1";
	testResult = UHMTUFrame::API_SRV_CheckAddressFormat(testSample);
	if (testResult == false) {
		wxMessageDialog(parent, testFailedMsg + testSample).ShowModal();
	}
	testSample = L"1.1.111.1";
	testResult = UHMTUFrame::API_SRV_CheckAddressFormat(testSample);
	if (testResult == false) {
		wxMessageDialog(parent, testFailedMsg + testSample).ShowModal();
	}
	testSample = L"1.1.1111.1";
	testResult = UHMTUFrame::API_SRV_CheckAddressFormat(testSample);
	if (testResult == true) {
		wxMessageDialog(parent, testFailedMsg + testSample).ShowModal();
	}
	testSample = L"1.1.1.1";
	testResult = UHMTUFrame::API_SRV_CheckAddressFormat(testSample);
	if (testResult == false) {
		wxMessageDialog(parent, testFailedMsg + testSample).ShowModal();
	}
	testSample = L"1.1.1.11";
	testResult = UHMTUFrame::API_SRV_CheckAddressFormat(testSample);
	if (testResult == false) {
		wxMessageDialog(parent, testFailedMsg + testSample).ShowModal();
	}
	testSample = L"1.1.1.111";
	testResult = UHMTUFrame::API_SRV_CheckAddressFormat(testSample);
	if (testResult == false) {
		wxMessageDialog(parent, testFailedMsg + testSample).ShowModal();
	}
	testSample = L"1.1.1.1111";
	testResult = UHMTUFrame::API_SRV_CheckAddressFormat(testSample);
	if (testResult == true) {
		wxMessageDialog(parent, testFailedMsg + testSample).ShowModal();
	}
	testSample = L"1";
	testResult = UHMTUFrame::API_SRV_CheckAddressFormat(testSample);
	if (testResult == true) {
		wxMessageDialog(parent, testFailedMsg + testSample).ShowModal();
	}
	testSample = L"1.";
	testResult = UHMTUFrame::API_SRV_CheckAddressFormat(testSample);
	if (testResult == true) {
		wxMessageDialog(parent, testFailedMsg + testSample).ShowModal();
	}
	testSample = L"1.1";
	testResult = UHMTUFrame::API_SRV_CheckAddressFormat(testSample);
	if (testResult == true) {
		wxMessageDialog(parent, testFailedMsg + testSample).ShowModal();
	}
	testSample = L"1.1.";
	testResult = UHMTUFrame::API_SRV_CheckAddressFormat(testSample);
	if (testResult == true) {
		wxMessageDialog(parent, testFailedMsg + testSample).ShowModal();
	}
	testSample = L"1.x";
	testResult = UHMTUFrame::API_SRV_CheckAddressFormat(testSample);
	if (testResult == true) {
		wxMessageDialog(parent, testFailedMsg + testSample).ShowModal();
	}
	testSample = L"abcdefg";
	testResult = UHMTUFrame::API_SRV_CheckAddressFormat(testSample);
	if (testResult == true) {
		wxMessageDialog(parent, testFailedMsg + testSample).ShowModal();
	}
	testSample = L"whatevergtwhatevergtwhatevergtwhatevergtwhatevergt46";
	testResult = UHMTUFrame::API_SRV_CheckAddressFormat(testSample);
	if (testResult == true) {
		wxMessageDialog(parent, testFailedMsg + testSample).ShowModal();
	}
}