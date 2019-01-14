#include "texture.h"



int loadTextureBox(){
	OpenFileDialog* openFileDialog1 = new OpenFileDialog();

	openFileDialog1->ShowDialog();
	openFileDialog1->FilterIndex = 1;
	openFileDialog1->Flags |= OFN_SHOWHELP;
	openFileDialog1->InitialDir = _T("C:\\Windows\\");
	openFileDialog1->Title = _T("Open Text File");
	printf(openFileDialog1->FileName);

	return 	loadTexture(openFileDialog1->FileName);

}


std::vector<int> loadDefaultTexture()
{
	std::vector<int> temptexture;
	int temp = SOIL_load_OGL_texture(
			"Data//brickwall_1.bmp",
			SOIL_LOAD_AUTO,
			SOIL_CREATE_NEW_ID,
			SOIL_FLAG_INVERT_Y
			);
	temptexture.push_back(temp);

		temp = SOIL_load_OGL_texture(
			"Data/tex1.bmp",
			SOIL_LOAD_AUTO,
			SOIL_CREATE_NEW_ID,
			SOIL_FLAG_INVERT_Y
			);
		temptexture.push_back(temp);
		return temptexture;
}

int loadTexture( TCHAR* pathname ){
	std::string s = "C:/Users/jermin/Documents/Visual Studio 2015/Projects/Project1Data//brickwall_1.bmp";
	std::string s2 = pathname;
	for (char& c : s2) {
		if (c == '\\') {
			c = '/';
		}
	}
	GLint temp = SOIL_load_OGL_texture(
		pathname,
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_INVERT_Y
		);


	std::cout << s.c_str() << std::endl;;
	std::cout << s2.c_str()<<std::endl;

		return temp;
}
