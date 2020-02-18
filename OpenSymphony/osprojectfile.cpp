#include "osprojectfile.h"

OSProjectFile::OSProjectFile()
{
	
}

int OSProjectFile::setProjectFileName(QString fileName) {
	int success = 0;
	//try {
		this->saveFileName = fileName;
	//}
	
	//catch(Exception e){
	//	success = e;
	//}
	
	return success;
}

int OSProjectFile::saveProjectFile() {
	//write out project data to txt file
	return 1;
}

