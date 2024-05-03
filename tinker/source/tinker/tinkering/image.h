class Image
{
public:
    ~Image();
    void load(const char* filePath);

    inline u32 getWidth() const { return m_Width; }
    inline u32 getHeight() const { return m_Height; }
    inline u32 getChannelsCount() const { return m_ChannelsCount; }
    inline const u8* getPixels() const { return m_Pixels; }

private:
    u8* m_Pixels;
    u32 m_Width;
    u32 m_Height;
    u32 m_ChannelsCount;
};