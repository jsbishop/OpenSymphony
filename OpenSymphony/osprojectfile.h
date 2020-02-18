#ifndef OSPROJECTFILE_H
#define OSPROJECTFILE_H

#include <QObject>


class OSProjectFile
{
	public:
		OSProjectFile();
		QString saveFileName;
		
		int saveProjectFile();
};

#endif // OSPROJECTFILE_H
