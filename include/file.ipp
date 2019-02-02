template<typename T> File& operator<<(File& f, T data)
{
	f.file << data;
	return f;
}
