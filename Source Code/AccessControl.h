/*
 * File: AccessControl.h
 * ----------
 * For file protection and access control
 */
/*
 * File: AccessControl.h
 * ----------
 * For file protection and access control
 */
#include <ctime>
#include <cstring>
#pragma once

typedef struct access
{
	int creator;

	int lastEditor;

	int accessby;

	time_t lastModification;

	time_t lastAccess;

	bool visibility;

	char mode[10]; // equivalent to the boolean variables following

	// It seems we won't have groups, so 3, 4, 5 are of no use

	//bool RUSR, WUSR, XUSR;

	//bool RGRP, WGRP, XGRP;

	//bool ROTH, WOTH, XOTH;
} Access;
