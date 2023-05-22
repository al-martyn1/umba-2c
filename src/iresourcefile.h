namespace embeddede_resources
{

struct IResourceFile
{

    virtual const unsigned char* data() = 0;
    virtual size_t               size() = 0;
    virtual const char*          mimeType()     { return 0; }
    virtual const char*          lastModified() { return 0; }
    virtual const char*          resourceFilename() { return 0; }
    virtual int                  resourceFilenameCompare( const char* cmpTo ) { return 0; }


}; // struct IResourceFile

}
