#pragma once



class String
{
public:
    String();
    String(const String& other);
    String& operator=(const String& other);
    String(String&& other);
    String& operator=(String&& other);
    ~String();

    String(const char* str);
};