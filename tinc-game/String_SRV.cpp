#include "String_SRV.h"

std::wstring String_SRV::ForceToWstring(std::string input)
{
	return std::wstring(input.begin(), input.end());
}

/// <summary>
/// CP437 able to type characters (32 - 255)
/// Control characters (0 - 31)
/// Standard character set (32 - 127)
/// Extended character set (128 - 255)
/// </summary>
const std::wstring cp437Table = L" !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~␡ÇüéâäàåçêëèïîìÄÅÉæÆôöòûùÿÖÜ¢£¥₧ƒáíóúñÑªº¿⌐¬½¼¡«»░▒▓│┤╡╢╖╕╣║╗╝╜╛┐└┴┬├─┼╞╟╚╔╩╦╠═╬╧╨╤╥╙╘╒╓╫╪┘┌█▄▌▐▀αßΓπΣσµτΦΘΩδ∞φε∩≡±≥≤⌠⌡÷≈°∙·√ⁿ²■";
bool String_SRV::IsCp437(std::wstring input)
{
	bool result = true;
	for (int i = 0; i < input.length(); i++) {

		if (cp437Table.find(input[i]) == std::string::npos) {
			result = false;
			break;
		}
	}
	return result;
}
