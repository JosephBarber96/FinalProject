#pragma once

#define TestDLL __declspec(dllexport)

extern "C"
{
	TestDLL int Add(int a, int b);
	TestDLL void Sort(int ia[], int size);
}