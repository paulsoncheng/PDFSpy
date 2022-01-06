// PDFSpy.cpp : 定义控制台应用程序的入口点。
//
/**
author:			paulson
createTime:		2021-12-15
Tel or qq:		911477285
*/
#include "stdafx.h"
#include "utils\PDF.h"
#include <string>

using namespace std;
void ShowTitle()
{
	printf("\r\n");
	printf("PDFSpy is a tiny tool to browse all objs in pdf file.\n");
	printf("Any Copyright reserved.Anyone can use this software freely\n");
	printf("So enjoy it!!!!");
	printf("\r\n");
	printf("\r\n");
	printf("PDFSpy 是一款迷你解析pdf工具，它仅用来显示pdf中简介对象，以及对象的数据。");
	printf("保留所有权。但任何人可以自由使用本软件。\n");
	
}
void ShowUsage()
{
	printf("\r\n");
	ShowTitle();
	
	printf("PDFSpy [-v] [pdffile]\n");
	printf("	-v --version show version\n");

	printf("e.g: PDFSpy  d:\\doc\\test.pdf\n");
	printf("e.g: PDFSpy -v\n");
	printf("e.g: PDFSpy --version\n");
	printf("\r\n");

	return;
}
int main(int argc, char *argv[])
{
	if (argc < 2 )
	{
		ShowUsage();
		return 1;
	}
	ShowTitle();
	if (argc == 2 && (strcmp(argv[1], "-v") == 0 || strcmp(argv[1], "--version")==0))
	{
		printf("PDFSpy 1.0.0.1\n");
	}
	else {
		CPDF pdf(argv[1]);
		pdf.VerifySign();
	}
	printf("\npress any key exit!!");
	getchar();
    return 0;
}

