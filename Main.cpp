#include<iostream>
#include<vector>
#include<string>
#include<fstream>
#include "BeamDriver.h"

#ifdef _MSC_VER
#define SSCANF sscanf_s
#else
#define SSCANF sscanf
#endif

std::vector<Safe> UserInputSetupSafes()
{
	std::vector<Safe> vSafes;
	std::string line;
	int rowCnt, colCnt, mLines, nLines;
	int row, col;

	while (getline(std::cin, line))
	{
		if (line.empty()) { break; }

		if (SSCANF(line.c_str(), "%d %d %d %d", &rowCnt, &colCnt, &mLines, &nLines) != 4)
		{
			std::cout << "Unknown input. Please start over.\n";
			break;
		}
		if (!(rowCnt, colCnt <= 1000000 && rowCnt, colCnt >= 0 && \
			mLines, nLines <= 200000 && mLines, nLines >= 0))
		{
			std::cout << "Input out of range. Please start over.\n";
			break;
		}

		Safe safe(rowCnt, colCnt);

		for (auto i = 0; i < mLines; i++)
		{
			getline(std::cin, line);
			SSCANF(line.c_str(), "%d %d", &row, &col);
			Position pos(row - 1, col - 1); 
			safe.SetMirrorInGrid(pos, ForwardSlantMirror);
		}

		for (auto j = 0; j < nLines; j++)
		{
			getline(std::cin, line);
			SSCANF(line.c_str(), "%d %d", &row, &col);
			Position pos(row - 1, col - 1);
			safe.SetMirrorInGrid(pos, BackSlantMirror);
		}
		vSafes.push_back(safe);
	}
	return vSafes;
}


Position GetLexSmallestMirror(const std::vector<MirrorObj>& mirrorObjects)
{
	Position smallestPos;

	if (!mirrorObjects.empty())
	{
		smallestPos = mirrorObjects[0].pos;
		for (auto m : mirrorObjects)
		{
			if (m.pos < smallestPos)
			{
				smallestPos = m.pos;
			}
		}
	}
	return smallestPos;
}

void PrintResults(const std::vector<MirrorObj>& missingMirrors, bool safeOpenSuccess)
{
	if (!safeOpenSuccess && missingMirrors.empty())
	{
		std::cout << "Impossible";
	}
	else if (safeOpenSuccess)
	{
		if (missingMirrors.empty())
		{
			std::cout << "0";
		}
		else
		{
			auto mirrorPos = GetLexSmallestMirror(missingMirrors);
			std::cout << missingMirrors.size() << " "
				<< mirrorPos.row + 1 << " " << mirrorPos.col + 1;
		}
	}
	std::cout << std::endl;

}

int main(int argc, char* argv[])
{
	try
	{
		auto vSafes = UserInputSetupSafes();
		auto count = 0;

		for (auto safe : vSafes)
		{
			BeamDriver bd(safe);
			std::vector<MirrorObj> missingMirrors;

			auto safeOpenSuccess = bd.BeamOpenSafeMissingMirrorFind(missingMirrors);

			std::cout << "Case " << ++count << " : ";
			PrintResults(missingMirrors, safeOpenSuccess);
			
		}
	}
	catch (std::exception& exp)
	{
		std::cout << "Error: "<<exp.what() << std::endl;
		return -1;
	}

	return 0;
}