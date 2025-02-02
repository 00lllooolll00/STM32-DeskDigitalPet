#include "Emoji.h"

/**
 * @brief 初始化表情
 * 
 */
void Emoji_Init(void)
{
    OLED_Init();

    OLED_Clear();
    OLED_ShowImage(0,0,128,64,Emoji_Sleep);
    OLED_Update();
}

const uint8_t *pEmoji_Buf = 0;//emoji上一次的值
/**
 * @brief 更换表情
 * 
 * @param Emoji 可取值:Emoji_Ashamed,Emoji_Cry,Emoji_Cute,Emoji_Happy,Emoji_Kiss,Emoji_Laugh,
 *                    Emoji_Normal,Emoji_Scared,Emoji_Sleep
 */
void Emoji_Turn(const uint8_t *Emoji)
{
    if(pEmoji_Buf == Emoji)
    {
        return;
    }
    else
    {
        OLED_Clear();
        OLED_ShowImage(0,0,128,64,Emoji);
        OLED_Update();
        pEmoji_Buf = Emoji;
    }
}
