#include <stdint.h>
#define PARSE_ERROR 0
typedef unsigned char byte;
/*
Unicode���ŷ�Χ     |        UTF-8���뷽ʽ
(ʮ������)        |              �������ƣ�
----------------------+---------------------------------------------
0000 0000-0000 007F | 0xxxxxxx
0000 0080-0000 07FF | 110xxxxx 10xxxxxx
0000 0800-0000 FFFF | 1110xxxx 10xxxxxx 10xxxxxx
0001 0000-0010 FFFF | 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
*/
/**
 * @brief utf8����תunicode�ַ���(usc4)�����֧��4�ֽ�utf8���룬(4�ֽ����������պ�����Ϊ��Ƨ��)
 * @param *utf8 utf8�䳤�����ֽڼ�1~4���ֽ�
 * @param *unicode utf8����תunicode�ַ�����������4���ֽڣ����ص��ֽ�����utf8������һ��
 * @return length 0��utf8�����쳣��others������utf8���볤��
 */
uint8_t UTF8ToUnicode(uint8_t *utf8, uint32_t *unicode) {
    const uint8_t lut_size = 3;
    const uint8_t length_lut[] = {2, 3, 4};
    const uint8_t range_lut[] = {0xE0, 0xF0, 0xF8};
    const uint8_t mask_lut[] = {0x1F, 0x0F, 0x07};

    uint8_t length = 0;
    byte b = *(utf8 + 0);
    uint32_t i = 0;

    if(utf8 == NULL) {
        *unicode = 0;
        return PARSE_ERROR;
    }
    // utf8�������ASCII����,ʹ��0xxxxxx ��ʾ00~7F
    if(b < 0x80) {
        *unicode =  b;
        return 1;
    }
    // utf8������ISO8859-1 ASCII��չ�ַ���
    // ͬʱ���֧�ֱ���6���ֽڼ�1111110X
    if(b < 0xC0 || b > 0xFD) {
        *unicode = 0;
        return PARSE_ERROR;
    }
    for(i = 0; i < lut_size; i++) {
        if(b < range_lut[i]) {
            *unicode = b & mask_lut[i];
            length = length_lut[i];
            break;
        }
    }
    // �������ֽڵ�utf8���벻���н���
    if(length == 0) {
        *unicode = 0;
        return PARSE_ERROR;
    }
    // ȡ�����ֽ�����
    for(i = 1; i < length; i++ ) {
        b = *(utf8 + i);
        // ���ֽ�utf8��������ֽڷ�Χ10xxxxxx?~?10111111?
        if( b < 0x80 || b > 0xBF ) {
            break;
        }
        *unicode <<= 6;
        // ?00111111?
        *unicode |= (b & 0x3F);
    }
    // ����У��
    return (i < length) ? PARSE_ERROR : length;
}
/**
 * @brief 4�ֽ�unicode(usc4)�ַ���תutf16����
 * @param unicode unicode�ַ�ֵ
 * @param *utf16 utf16������
 * @return utf16���ȣ�(2�ֽ�)��λ
 */
uint8_t UnicodeToUTF16(uint32_t unicode, uint16_t *utf16) {
    // Unicode��Χ U+000~U+FFFF
    // utf16���뷽ʽ��2 Byte�洢����������Unicodeֵ
    if(unicode <= 0xFFFF) {
		if(utf16 != NULL) {
			*utf16 = (unicode & 0xFFFF);
		}
		return 1;
	}else if( unicode <= 0xEFFFF ) {
		if( utf16 != NULL ) {
		    // ��10λ
		    *(utf16 + 0) = 0xD800 + (unicode >> 10) - 0x40;
            // ��10λ
		    *(utf16 + 1) = 0xDC00 + (unicode &0x03FF);
		}
		return 2;
	}
    return 0;
}
