#include "openfile.h"

OpenFileDialog::OpenFileDialog(void)
{
	this->DefaultExtension = 0;
	this->FileName = new TCHAR[MAX_PATH];
	this->Filter = 0;
	this->FilterIndex = 0;
	this->Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	this->InitialDir = 0;
	this->Owner = 0;
	this->Title = 0;
}

bool OpenFileDialog::ShowDialog()
{
	OPENFILENAME filenames;
	ZeroMemory(&filenames, sizeof(filenames));
	filenames.lStructSize = sizeof(filenames);
	filenames.hwndOwner = this->Owner;
	filenames.lpstrDefExt = this->DefaultExtension;
	filenames.lpstrFile = this->FileName;
	filenames.lpstrFile[0] = '\0';
	filenames.nMaxFile = MAX_PATH;
	filenames.lpstrFilter = this->Filter;
	filenames.nFilterIndex = this->FilterIndex;
	filenames.lpstrInitialDir = this->InitialDir;
	filenames.lpstrTitle = this->Title;
	filenames.Flags = this->Flags;
	GetOpenFileName(&filenames);
	if (_tcslen(this->FileName) == 0) return false;

	return true;
}
