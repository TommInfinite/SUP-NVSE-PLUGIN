
DEFINE_COMMAND_ALT_PLUGIN(ExcelWriteFloat, XLSWriteFloat, "", 0, 5, kParams_Tomm_ExcelWriteFloat)
DEFINE_COMMAND_ALT_PLUGIN(ExcelWriteString,XLSWriteString, "", 0, 5, kParams_Tomm_ExcelWriteString)
DEFINE_COMMAND_ALT_PLUGIN(ExcelReadFloat,XLSReadFloat, "", 0, 4, kParams_Tomm_ExcelReadFloat)
DEFINE_COMMAND_ALT_PLUGIN(ExcelReadString,XLSReadString, "", 0, 4, kParams_Tomm_ExcelReadString)
DEFINE_COMMAND_ALT_PLUGIN(ExcelGetWorkSheetCount,XLSGetWorkSheetCount, "", 0, 1, kParams_Tomm_ExcelGetWorkSheetCount)
DEFINE_COMMAND_ALT_PLUGIN(ExcelGetRowCount,XLSGetRowCount, "", 0, 2, kParams_Tomm_ExcelGetRowCount)
DEFINE_COMMAND_ALT_PLUGIN(ExcelGetColCount,XLSGetColCount, "", 0, 2, kParams_Tomm_ExcelGetColCount)
DEFINE_COMMAND_ALT_PLUGIN(ExcelSetSheetName,XLSSetSheetName, "", 0, 3, kParams_Tomm_SetSheetName)
DEFINE_COMMAND_ALT_PLUGIN(ExcelGetCellValueType,XLSGetCellValueType, "", 0, 4, kParams_Tomm_ExcelGetCellValueType)
DEFINE_COMMAND_ALT_PLUGIN(ExcelEraseCell,XLSEraseCell, "", 0, 4, kParams_Tomm_ExcelEraseCell)
DEFINE_COMMAND_ALT_PLUGIN(ExcelEraseSheet,XLSEraseSheet, "", 0, 2, kParams_Tomm_ExcelEraseSheet)




#include "ExcelFormat.h"
using namespace ExcelFormat;
#include <windows.h>
#include <shellapi.h>
#include <crtdbg.h>



bool Cmd_ExcelWriteFloat_Execute(COMMAND_ARGS)
{
	char s_FilePath[MAX_PATH]{};
	int iWorkSheet = 0, iRow = 0, iCol = 0, iTotalWorkSheets = 0;
	float fValue = 0;
	BasicExcelCell* cell = NULL;
	BasicExcelWorksheet* sheet = NULL;

	BasicExcel xls;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &s_FilePath, &iWorkSheet, &iRow, &iCol, &fValue))
	{

		sprintf(s_FilePath, "%s%s", s_FilePath, ".xls");

		if (FileExists(s_FilePath))
		{
			//_MESSAGE("File exists %s", s_FilePath);
			if (xls.Load(s_FilePath))
			{
				//_MESSAGE("File loaded %s", s_FilePath);
			}
			else {
				*result = -1;
				return true;
			}
		}
		else { 
			//_MESSAGE("Creating %s", s_FilePath);
			xls.New(iWorkSheet + 1);
		}
		
		iTotalWorkSheets = xls.GetTotalWorkSheets();
		//_MESSAGE("Num of before::%d", iTotalWorkSheets);

		if (iWorkSheet > iTotalWorkSheets)
		{
			int iTemp = iTotalWorkSheets;
			while (iTemp <= iWorkSheet)
			{
				xls.AddWorksheet(-1);
				iTemp += 1;
			}

		}

		//_MESSAGE("Num of sheets::%d",xls.GetTotalWorkSheets());

		sheet = xls.GetWorksheet(iWorkSheet);

		if (sheet)
		{
			cell = sheet->Cell(iRow, iCol);
			cell->SetDouble(fValue);
			*result = 1;
		}

		xls.SaveAs(s_FilePath);
	}

	return true;
}



bool Cmd_ExcelWriteString_Execute(COMMAND_ARGS)
{
	char s_FilePath[MAX_PATH]{}, s_String[0x4000]{};
	int iWorkSheet = 0, iRow = 0, iCol = 0, iTotalWorkSheets = 0;
	BasicExcelCell* cell = NULL;
	BasicExcelWorksheet* sheet = NULL;

	BasicExcel xls;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &s_FilePath, &iWorkSheet, &iRow, &iCol, &s_String))
	{

		sprintf(s_FilePath, "%s%s", s_FilePath, ".xls");

		if (FileExists(s_FilePath))
		{
			//_MESSAGE("File exists %s", s_FilePath);
			if (xls.Load(s_FilePath))
			{
				//_MESSAGE("File loaded %s", s_FilePath);
			}
			else {
				*result = -1;
				return true;
			}
		}
		else {
			//_MESSAGE("Creating %s", s_FilePath);
			xls.New(iWorkSheet + 1);
		}

		iTotalWorkSheets = xls.GetTotalWorkSheets();
		//_MESSAGE("Num of before::%d", iTotalWorkSheets);

		if (iWorkSheet > iTotalWorkSheets)
		{
			int iTemp = iTotalWorkSheets;
			while (iTemp <= iWorkSheet)
			{
				xls.AddWorksheet(-1);
				iTemp += 1;
			}

		}

		//_MESSAGE("Num of sheets::%d",xls.GetTotalWorkSheets());

		sheet = xls.GetWorksheet(iWorkSheet);

		if (sheet)
		{
			cell = sheet->Cell(iRow, iCol);
			cell->SetString(s_String);
			*result = 1;
		}

		xls.SaveAs(s_FilePath);
	}

	return true;
}







bool Cmd_ExcelReadFloat_Execute(COMMAND_ARGS)
{
	char s_FilePath[MAX_PATH]{};
	int iWorkSheet = 0, iRow = 0, iCol = 0, iTotalWorkSheets = 0;
	BasicExcelCell* cell = NULL;
	BasicExcelWorksheet* sheet = NULL;

	BasicExcel xls;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &s_FilePath, &iWorkSheet, &iRow, &iCol))
	{

		sprintf(s_FilePath, "%s%s", s_FilePath, ".xls");

		if (FileExists(s_FilePath))
		{
			//_MESSAGE("File exists %s", s_FilePath);
			if (xls.Load(s_FilePath))
			{
				//_MESSAGE("File loaded %s", s_FilePath);
			}
			else {
				*result = -1;
				return true;
			}
		}
		else {
			*result = -1;
			return true;
		}

		iTotalWorkSheets = xls.GetTotalWorkSheets();

		if (iWorkSheet > iTotalWorkSheets)
		{
			*result = -1;
			xls.Close();
			return true;
		}

		sheet = xls.GetWorksheet(iWorkSheet);

		if (sheet)
		{
			cell = sheet->Cell(iRow, iCol);
			*result = cell->GetDouble();

		}
		xls.Close();
	}



	return true;
}




bool Cmd_ExcelReadString_Execute(COMMAND_ARGS)
{
	char s_FilePath[MAX_PATH]{};
	int iWorkSheet = 0, iRow = 0, iCol = 0, iTotalWorkSheets = 0;
	BasicExcelCell* cell = NULL;
	BasicExcelWorksheet* sheet = NULL;

	BasicExcel xls;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &s_FilePath, &iWorkSheet, &iRow, &iCol))
	{

		sprintf(s_FilePath, "%s%s", s_FilePath, ".xls");

		if (FileExists(s_FilePath))
		{
			//_MESSAGE("File exists %s", s_FilePath);
			if (xls.Load(s_FilePath))
			{
				//_MESSAGE("File loaded %s", s_FilePath);
			}
			else {
				*result = -1;
				return true;
			}
		}
		else {
			*result = -1;
			return true;
		}

		iTotalWorkSheets = xls.GetTotalWorkSheets();

		if (iWorkSheet > iTotalWorkSheets)
		{
			*result = -1;
			xls.Close();
			return true;
		}

		sheet = xls.GetWorksheet(iWorkSheet);

		if (sheet)
		{
			cell = sheet->Cell(iRow, iCol);
			StrIfc->Assign(PASS_COMMAND_ARGS, cell->GetString());

		}

		xls.Close();
	}



	return true;
}



bool Cmd_ExcelGetWorkSheetCount_Execute(COMMAND_ARGS)
{
	char s_FilePath[MAX_PATH]{};
	BasicExcelWorksheet* sheet = NULL;

	BasicExcel xls;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &s_FilePath))
	{

		sprintf(s_FilePath, "%s%s", s_FilePath, ".xls");

		if (FileExists(s_FilePath))
		{
			//_MESSAGE("File exists %s", s_FilePath);
			if (xls.Load(s_FilePath))
			{
				//_MESSAGE("File loaded %s", s_FilePath);
			}
			else {
				*result = -1;
				return true;
			}
		}
		else {
			*result = -1;
			return true;
		}

		*result = xls.GetTotalWorkSheets();
		xls.Close();
	}


	return true;
}



bool Cmd_ExcelGetRowCount_Execute(COMMAND_ARGS)
{
	char s_FilePath[MAX_PATH]{};
	int iWorkSheet = 0, iTotalWorkSheets = 0;
	BasicExcelWorksheet* sheet = NULL;

	BasicExcel xls;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &s_FilePath, &iWorkSheet))
	{

		sprintf(s_FilePath, "%s%s", s_FilePath, ".xls");

		if (FileExists(s_FilePath))
		{
			//_MESSAGE("File exists %s", s_FilePath);
			if (xls.Load(s_FilePath))
			{
				//_MESSAGE("File loaded %s", s_FilePath);
			}
			else {
				*result = -1;
				return true;
			}
		}
		else {
			*result = -1;
			return true;
		}

		iTotalWorkSheets = xls.GetTotalWorkSheets();

		if (iWorkSheet > iTotalWorkSheets)
		{
			*result = -1;
			xls.Close();
			return true;
		}

		sheet = xls.GetWorksheet(iWorkSheet);

		if (sheet)
		{
			*result = sheet->GetTotalRows();
		}

		xls.Close();
	}
	return true;
}


bool Cmd_ExcelGetColCount_Execute(COMMAND_ARGS)
{
	char s_FilePath[MAX_PATH]{};
	int iWorkSheet = 0, iTotalWorkSheets = 0;
	BasicExcelWorksheet* sheet = NULL;

	BasicExcel xls;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &s_FilePath, &iWorkSheet))
	{

		sprintf(s_FilePath, "%s%s", s_FilePath, ".xls");

		if (FileExists(s_FilePath))
		{
			//_MESSAGE("File exists %s", s_FilePath);
			if (xls.Load(s_FilePath))
			{
				//_MESSAGE("File loaded %s", s_FilePath);
			}
			else {
				*result = -1;
				return true;
			}
		}
		else {
			*result = -1;
			return true;
		}

		iTotalWorkSheets = xls.GetTotalWorkSheets();

		if (iWorkSheet > iTotalWorkSheets)
		{
			*result = -1;
			xls.Close();
			return true;
		}

		sheet = xls.GetWorksheet(iWorkSheet);

		if (sheet)
		{
			*result = sheet->GetTotalCols();
		}

		xls.Close();
	}
	return true;
}



bool Cmd_ExcelSetSheetName_Execute(COMMAND_ARGS)
{
	char s_FilePath[MAX_PATH]{}, s_SheetName[0x100];
	int iWorkSheet = 0, iTotalWorkSheets = 0;
	BasicExcelCell* cell = NULL;
	BasicExcelWorksheet* sheet = NULL;

	BasicExcel xls;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &s_FilePath, &iWorkSheet, &s_SheetName))
	{

		sprintf(s_FilePath, "%s%s", s_FilePath, ".xls");

		if (FileExists(s_FilePath))
		{
			//_MESSAGE("File exists %s", s_FilePath);
			if (xls.Load(s_FilePath))
			{
				//_MESSAGE("File loaded %s", s_FilePath);
			}
			else {
				*result = -1;
				return true;
			}
		}
		else {
			*result = -1;
			return true;
		}

		iTotalWorkSheets = xls.GetTotalWorkSheets();

		if (iWorkSheet > iTotalWorkSheets)
		{
			*result = -1;
			xls.Close();
			return true;
		}

		sheet = xls.GetWorksheet(iWorkSheet);

		if (sheet)
		{
			if (NUM_ARGS > 2)
			{

				int iLenght = strlen(s_SheetName);
				
				if (iLenght > 30)
				{
					while (iLenght > 30)
					{
						s_SheetName[iLenght] = 0;
						iLenght -= 1;
					}
				}
				sheet->Rename(s_SheetName);
				xls.SaveAs(s_FilePath);
			}
			else {

				sheet->GetSheetName(s_SheetName);
				StrIfc->Assign(PASS_COMMAND_ARGS, s_SheetName);

			}


		}

		xls.Close();
	}
	return true;
}




//0 =UNDEFINED,  1= INT,  2= DOUBLE,  3= STRING,  4= WSTRING,  5= FORMULA
bool Cmd_ExcelGetCellValueType_Execute(COMMAND_ARGS)
{
	char s_FilePath[MAX_PATH]{};
	int iWorkSheet = 0, iRow = 0, iCol = 0, iTotalWorkSheets = 0;
	BasicExcelCell* cell = NULL;
	BasicExcelWorksheet* sheet = NULL;

	BasicExcel xls;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &s_FilePath, &iWorkSheet, &iRow, &iCol))
	{

		sprintf(s_FilePath, "%s%s", s_FilePath, ".xls");

		if (FileExists(s_FilePath))
		{
			//_MESSAGE("File exists %s", s_FilePath);
			if (xls.Load(s_FilePath))
			{
				//_MESSAGE("File loaded %s", s_FilePath);
			}
			else {
				*result = -1;
				return true;
			}
		}
		else {
			*result = -1;
			return true;
		}

		iTotalWorkSheets = xls.GetTotalWorkSheets();

		if (iWorkSheet > iTotalWorkSheets)
		{
			*result = -1;
			xls.Close();
			return true;
		}

		sheet = xls.GetWorksheet(iWorkSheet);

		if (sheet)
		{
			cell = sheet->Cell(iRow, iCol);
			*result = cell->Type();
		}
		xls.Close();
	}



	return true;
}



bool Cmd_ExcelEraseCell_Execute(COMMAND_ARGS)
{
	char s_FilePath[MAX_PATH]{};
	int iWorkSheet = 0, iRow = 0, iCol = 0, iTotalWorkSheets = 0;
	BasicExcelCell* cell = NULL;
	BasicExcelWorksheet* sheet = NULL;

	BasicExcel xls;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &s_FilePath, &iWorkSheet, &iRow, &iCol))
	{

		sprintf(s_FilePath, "%s%s", s_FilePath, ".xls");

		if (FileExists(s_FilePath))
		{
			//_MESSAGE("File exists %s", s_FilePath);
			if (xls.Load(s_FilePath))
			{
				//_MESSAGE("File loaded %s", s_FilePath);
			}
			else {
				*result = -1;
				return true;
			}
		}
		else {
			*result = -1;
			return true;
		}

		iTotalWorkSheets = xls.GetTotalWorkSheets();

		if (iWorkSheet > iTotalWorkSheets)
		{
			*result = -1;
			xls.Close();
			return true;
		}

		sheet = xls.GetWorksheet(iWorkSheet);

		if (sheet)
		{
			cell = sheet->Cell(iRow, iCol);
			cell->EraseContents();
			*result = 1;
		}
		xls.Save();
		xls.Close();
	}



	return true;
}



bool Cmd_ExcelEraseSheet_Execute(COMMAND_ARGS)
{
	char s_FilePath[MAX_PATH]{};
	int iWorkSheet = 0, iTotalWorkSheets = 0;
	BasicExcelCell* cell = NULL;
	BasicExcelWorksheet* sheet = NULL;

	BasicExcel xls;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &s_FilePath, &iWorkSheet))
	{

		sprintf(s_FilePath, "%s%s", s_FilePath, ".xls");

		if (FileExists(s_FilePath))
		{
			//_MESSAGE("File exists %s", s_FilePath);
			if (xls.Load(s_FilePath))
			{
				//_MESSAGE("File loaded %s", s_FilePath);
			}
			else {
				*result = -1;
				return true;
			}
		}
		else {
			*result = -1;
			return true;
		}

		iTotalWorkSheets = xls.GetTotalWorkSheets();

		if (iWorkSheet > iTotalWorkSheets)
		{
			*result = -1;
			xls.Close();
			return true;
		}

		sheet = xls.GetWorksheet(iWorkSheet);

		int iTotalRows = sheet->GetTotalRows();
		int iTotalColumns = sheet->GetTotalCols();
		int iCountRows = 0, iCountCols = 0;

		while(iCountRows < iTotalRows)
		{
			iCountCols = 0;
			while (iCountCols < iTotalColumns)
			{
				cell = sheet->Cell(iCountRows, iCountCols);
				cell->EraseContents();
				iCountCols+=1;
			}
			iCountRows+=1;
		}

		*result = 1;
		xls.Save();
		xls.Close();
	}

	return true;
}





//char Name[0x100];
//sprintf(Name, "%d", iWorkSheet - 1);
//sheet->Rename(Name);