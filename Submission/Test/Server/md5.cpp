//////////

std::ostream& operator<<(std::ostream& out, MD5 md5)
{
	return out << md5.hexdigest();
}

//////////////////////////////

std::string md5(const std::string str)
{
	MD5 md5 = MD5(str);

	return md5.hexdigest();
}


#include <iostream>

#ifdef TEST_MD5

int main(int argc, char *argv[])
{
	std::cout << "md5 of 'grape': " << md5("grape")<<std::endl;
	return 0;
}

#endif