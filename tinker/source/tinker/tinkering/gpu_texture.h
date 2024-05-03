class GPUTexture
{
public:
    ~GPUTexture();
    void make(const Image& image);

    inline u32 getWidth() const { return m_Width; }
    inline u32 getHeight() const { return m_Height; }

private:
    u32 m_Width;
    u32 m_Height;
};