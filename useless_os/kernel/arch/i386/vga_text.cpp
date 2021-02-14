#include <string.h>
#include <stdint.h>

#include <kernel/vga_text.h>

// VGA text mode, buffer accessible with memory-mapped I/O
// with the address 0xb8000.
namespace vga
{
    using ColorCode = uint8_t;

    static constexpr ColorCode make_color_code(Color foreground, Color background)
    {
        return ColorCode(uint8_t(foreground) | (uint8_t(background) << 4u));
    }

    static constexpr uint32_t kBufferWidth  = 80;
    static constexpr uint32_t kBufferHeight = 25;
    constexpr ColorCode kDefaultColor = make_color_code(Color::White, Color::Black);

    struct ScreenChar
    {
        uint8_t ascii_;
        ColorCode color_;

        static constexpr ScreenChar from_char(char ch, ColorCode color = kDefaultColor)
        {
            return ScreenChar(uint8_t(ch), color);
        }
    };

    static_assert(sizeof(ScreenChar) == 2);

    struct Buffer
    {
        uint32_t row_;
        uint32_t column_;
        ScreenChar* chars_;
    };

    Buffer g_vga_text;

    void terminal_initialize()
    {
        g_vga_text.row_ = 0;
        g_vga_text.column_ = 0;
        g_vga_text.chars_ = reinterpret_cast<ScreenChar*>(0xb8000);

        constexpr auto char_ = ScreenChar::from_char(' ');
        for (uint32_t i = 0; i < (kBufferWidth * kBufferHeight); ++i)
        {
            g_vga_text.chars_[i] = char_;
        }
    }

    void terminal_putchar(char c)
    {
        const uint32_t index = g_vga_text.row_ * kBufferWidth + g_vga_text.column_;
        g_vga_text.chars_[index] = ScreenChar::from_char(c);

        if (++g_vga_text.column_ == kBufferWidth)
        {
            g_vga_text.column_ = 0;
            if (++g_vga_text.row_ == kBufferHeight)
            {
                g_vga_text.row_ = 0;
            }
        }
    }

    void terminal_write(const char* data, size_t size)
    {
        for (size_t i = 0; i < size; ++i)
        {
            terminal_putchar(data[i]);
        }
    }

    void terminal_write_string(const char* data)
    {
        terminal_write(data, strlen(data));
    }
} // namespace

