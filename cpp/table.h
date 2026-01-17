#pragma once
/**
 * table.h
 */

#ifndef TABLE_H
#define TABLE_H

#include <algorithm>
#include <cmath>
#include <vector>
#include <string>
#include <cpp/printfw.h>
#include <windows.h>
#include <codecvt>

std::wstring TableCenterText(const std::wstring& s, int width = -1, bool right=true) {
	if (width < 0) width = static_cast<int>(s.length() + 2);
	if (static_cast<int>(s.length()) >= width) return s;
	int total_padding = width - static_cast<int>(s.length());
	int left_padding = total_padding / 2;
	int right_padding = total_padding - left_padding;
	std::wstring padded_string(left_padding, L' ');
	padded_string += s;
	if (right) padded_string += std::wstring(right_padding, L' ');
	return padded_string;
}
std::wstring TableStringToWString(const std::string_view text) {
	if (text.empty()) return std::wstring();
	int size_needed = MultiByteToWideChar(CP_UTF8, 0, text.data(), static_cast<int>(text.size()), nullptr, 0);
	std::wstring wstrTo(size_needed, 0);
	MultiByteToWideChar(CP_UTF8, 0, text.data(), static_cast<int>(text.size()), &wstrTo[0], size_needed);
	return wstrTo;
}
std::string TableWStringToString(const std::wstring_view text) {
	if (text.empty()) return std::string();
	int size_needed = WideCharToMultiByte(CP_UTF8, 0, text.data(), static_cast<int>(text.size()), nullptr, 0, nullptr, nullptr);
	std::string wstrTo(size_needed, 0);
	WideCharToMultiByte(CP_UTF8, 0, text.data(), static_cast<int>(text.size()), &wstrTo[0], size_needed, nullptr, nullptr);
	return wstrTo;
}

int TableConsoleWidth() {
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	static HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleScreenBufferInfo(hConsole, &csbi);
	return csbi.srWindow.Right - csbi.srWindow.Left + 1;
}

/*
* Prints the items in a table-like strucure
* @param items The items to print
* @param maxwidth The % of console width to use (if < 0, treated as percentage)
* @param dynamic Whether all items should have the same width, like a grid, or not
*/
void PrintTable(const std::vector<std::wstring>& items, double maxwidth = 80, bool dynamic = false) {
	if (items.empty()) return;
	if (maxwidth > 1) maxwidth /= 100;
	// Get console width
	const int consoleWidth = TableConsoleWidth();
	const int padding = 2;

	if (dynamic) {

	}
	else {
		// Find the longest item (in characters)
		size_t longest = 0;
		for (const auto& item : items) {
			if (item.size() > longest) {
				longest = item.size();
			}
		}
		const int colWidth = static_cast<int>(longest) + padding * 2;
		const int usableWidth = static_cast<int>(consoleWidth * maxwidth);

		// Compute how many columns fit and clamp to at least 1
		int maxColsThatFit = max(1, usableWidth / colWidth);
		int cols = min(maxColsThatFit, static_cast<int>(items.size()));
		int rows = (static_cast<int>(items.size()) + cols - 1) / cols;

		// Build rows, filling empty cells with padded blanks so column widths remain constant
		std::vector<std::wstring> tableLines;
		const int borderContentWidth = 1 + cols * (colWidth + 1); // matches: "|" + (text + "|") * cols
		const int leftPaddingSpaces = max(0, (consoleWidth - borderContentWidth) / 2);
		const std::wstring leftSpace(leftPaddingSpaces, L' ');

		for (int r = 0; r < rows; ++r) {
			std::wstring line;
			for (int c = 0; c < cols; ++c) {
				// Use row-major indexing so the table fills left->right on each row
				int index = r * cols + c;
				if (index < static_cast<int>(items.size())) {
					const std::wstring text = TableCenterText(items[index], colWidth);
					line += text;
				}
				else {
					// empty cell: same width as a real cell so alignment holds
					line += TableCenterText(std::wstring(), colWidth);
				}
				line += L"|";
			}
			tableLines.push_back(leftSpace + L"|" + line);
		}

		// Draw border matching table width
		const std::wstring border = leftSpace + std::wstring(borderContentWidth, L'=');
		std::wcout << border << std::endl;
		printfwallj(tableLines, L"\n");
		std::wcout << std::endl << border << std::endl;
	}
	std::cout << std::endl;
}

#endif
