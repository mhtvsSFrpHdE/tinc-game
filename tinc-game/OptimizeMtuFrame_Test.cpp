#include "OptimizeMtuFrame.h"

void UHMTUFrameTest::TestCheckAddressFormat(wxWindow* parent) {
	auto testFailedMsg = std::wstring(L"Test failed: ");

	auto testSample = std::wstring(L"1.1.1.1");

	auto testResult = OptimizeMtuFrame::API_SRV_CheckAddressFormat(testSample);
	if (testResult.success == false) {
		wxMessageDialog(parent, testFailedMsg + testSample).ShowModal();
	}
	testSample = L"11.1.1.1";
	testResult = OptimizeMtuFrame::API_SRV_CheckAddressFormat(testSample);
	if (testResult.success == false) {
		wxMessageDialog(parent, testFailedMsg + testSample).ShowModal();
	}
	testSample = L"111.1.1.1";
	testResult = OptimizeMtuFrame::API_SRV_CheckAddressFormat(testSample);
	if (testResult.success == false) {
		wxMessageDialog(parent, testFailedMsg + testSample).ShowModal();
	}
	testSample = L"1111.1.1.1";
	testResult = OptimizeMtuFrame::API_SRV_CheckAddressFormat(testSample);
	if (testResult.success == true) {
		wxMessageDialog(parent, testFailedMsg + testSample).ShowModal();
	}
	testSample = L"1.1.1.1";
	testResult = OptimizeMtuFrame::API_SRV_CheckAddressFormat(testSample);
	if (testResult.success == false) {
		wxMessageDialog(parent, testFailedMsg + testSample).ShowModal();
	}
	testSample = L"1.11.1.1";
	testResult = OptimizeMtuFrame::API_SRV_CheckAddressFormat(testSample);
	if (testResult.success == false) {
		wxMessageDialog(parent, testFailedMsg + testSample).ShowModal();
	}
	testSample = L"1.111.1.1";
	testResult = OptimizeMtuFrame::API_SRV_CheckAddressFormat(testSample);
	if (testResult.success == false) {
		wxMessageDialog(parent, testFailedMsg + testSample).ShowModal();
	}
	testSample = L"1.1111.1.1";
	testResult = OptimizeMtuFrame::API_SRV_CheckAddressFormat(testSample);
	if (testResult.success == true) {
		wxMessageDialog(parent, testFailedMsg + testSample).ShowModal();
	}
	testSample = L"1.1.1.1";
	testResult = OptimizeMtuFrame::API_SRV_CheckAddressFormat(testSample);
	if (testResult.success == false) {
		wxMessageDialog(parent, testFailedMsg + testSample).ShowModal();
	}
	testSample = L"1.1.11.1";
	testResult = OptimizeMtuFrame::API_SRV_CheckAddressFormat(testSample);
	if (testResult.success == false) {
		wxMessageDialog(parent, testFailedMsg + testSample).ShowModal();
	}
	testSample = L"1.1.111.1";
	testResult = OptimizeMtuFrame::API_SRV_CheckAddressFormat(testSample);
	if (testResult.success == false) {
		wxMessageDialog(parent, testFailedMsg + testSample).ShowModal();
	}
	testSample = L"1.1.1111.1";
	testResult = OptimizeMtuFrame::API_SRV_CheckAddressFormat(testSample);
	if (testResult.success == true) {
		wxMessageDialog(parent, testFailedMsg + testSample).ShowModal();
	}
	testSample = L"1.1.1.1";
	testResult = OptimizeMtuFrame::API_SRV_CheckAddressFormat(testSample);
	if (testResult.success == false) {
		wxMessageDialog(parent, testFailedMsg + testSample).ShowModal();
	}
	testSample = L"1.1.1.11";
	testResult = OptimizeMtuFrame::API_SRV_CheckAddressFormat(testSample);
	if (testResult.success == false) {
		wxMessageDialog(parent, testFailedMsg + testSample).ShowModal();
	}
	testSample = L"1.1.1.111";
	testResult = OptimizeMtuFrame::API_SRV_CheckAddressFormat(testSample);
	if (testResult.success == false) {
		wxMessageDialog(parent, testFailedMsg + testSample).ShowModal();
	}
	testSample = L"1.1.1.1111";
	testResult = OptimizeMtuFrame::API_SRV_CheckAddressFormat(testSample);
	if (testResult.success == true) {
		wxMessageDialog(parent, testFailedMsg + testSample).ShowModal();
	}
	testSample = L"1";
	testResult = OptimizeMtuFrame::API_SRV_CheckAddressFormat(testSample);
	if (testResult.success == true) {
		wxMessageDialog(parent, testFailedMsg + testSample).ShowModal();
	}
	testSample = L"1.";
	testResult = OptimizeMtuFrame::API_SRV_CheckAddressFormat(testSample);
	if (testResult.success == true) {
		wxMessageDialog(parent, testFailedMsg + testSample).ShowModal();
	}
	testSample = L"1.1";
	testResult = OptimizeMtuFrame::API_SRV_CheckAddressFormat(testSample);
	if (testResult.success == true) {
		wxMessageDialog(parent, testFailedMsg + testSample).ShowModal();
	}
	testSample = L"1.1.";
	testResult = OptimizeMtuFrame::API_SRV_CheckAddressFormat(testSample);
	if (testResult.success == true) {
		wxMessageDialog(parent, testFailedMsg + testSample).ShowModal();
	}
	testSample = L"1.x";
	testResult = OptimizeMtuFrame::API_SRV_CheckAddressFormat(testSample);
	if (testResult.success == true) {
		wxMessageDialog(parent, testFailedMsg + testSample).ShowModal();
	}
	testSample = L"abcdefg";
	testResult = OptimizeMtuFrame::API_SRV_CheckAddressFormat(testSample);
	if (testResult.success == true) {
		wxMessageDialog(parent, testFailedMsg + testSample).ShowModal();
	}
	testSample = L"whatevergtwhatevergtwhatevergtwhatevergtwhatevergt46";
	testResult = OptimizeMtuFrame::API_SRV_CheckAddressFormat(testSample);
	if (testResult.success == true) {
		wxMessageDialog(parent, testFailedMsg + testSample).ShowModal();
	}
}