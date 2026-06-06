#include <cstdint>
#include <sstream>

#include <QChar>
#include <QDebug>
#include <Qt>
#include <xkbcommon/xkbcommon-keysyms.h>
#include <xkbcommon/xkbcommon.h>

static inline Qt::Key keysym_map(xkb_keysym_t keysym) {
  switch (keysym) {
  case XKB_KEY_ISO_Left_Tab:
    return Qt::Key_Backtab;
  case XKB_KEY_BackSpace:
    return Qt::Key_Backspace;
  case XKB_KEY_Tab:
    return Qt::Key_Tab;
  case XKB_KEY_Clear:
    return Qt::Key_Delete;
  case XKB_KEY_Return:
    return Qt::Key_Return;
  case XKB_KEY_Pause:
    return Qt::Key_Pause;
  case XKB_KEY_Sys_Req:
    return Qt::Key_SysReq;
  case XKB_KEY_Escape:
    return Qt::Key_Escape;
  case XKB_KEY_Home:
    return Qt::Key_Home;
  case XKB_KEY_Left:
    return Qt::Key_Left;
  case XKB_KEY_Up:
    return Qt::Key_Up;
  case XKB_KEY_Right:
    return Qt::Key_Right;
  case XKB_KEY_Down:
    return Qt::Key_Down;
  case XKB_KEY_Prior:
    return Qt::Key_PageUp;
  case XKB_KEY_Next:
    return Qt::Key_PageDown;
  case XKB_KEY_End:
    return Qt::Key_End;
  case XKB_KEY_Print:
    return Qt::Key_Print;
  case XKB_KEY_Insert:
    return Qt::Key_Insert;
  case XKB_KEY_Shift_L:
    return Qt::Key_Shift;
  case XKB_KEY_Shift_R:
    return Qt::Key_Shift;
  case XKB_KEY_Control_L:
    return Qt::Key_Control;
  case XKB_KEY_Control_R:
    return Qt::Key_Control;
  case XKB_KEY_Caps_Lock:
    return Qt::Key_CapsLock;
  case XKB_KEY_Shift_Lock:
    return Qt::Key_Shift;
  case XKB_KEY_Meta_L:
    return Qt::Key_Meta;
  case XKB_KEY_Meta_R:
    return Qt::Key_Meta;
  case XKB_KEY_Alt_L:
    return Qt::Key_Alt;
  case XKB_KEY_Alt_R:
    return Qt::Key_Alt;
  case XKB_KEY_Num_Lock:
    return Qt::Key_NumLock;
  case XKB_KEY_Scroll_Lock:
    return Qt::Key_ScrollLock;
  case XKB_KEY_Super_L:
    return Qt::Key_Super_L;
  case XKB_KEY_Super_R:
    return Qt::Key_Super_R;
  case XKB_KEY_Delete:
    return Qt::Key_Delete;
  case 0x1005FF60:
    return Qt::Key_SysReq;
  case 0x1007ff00:
    return Qt::Key_SysReq;
  case XKB_KEY_Menu:
    return Qt::Key_Menu;
  case XKB_KEY_Hyper_L:
    return Qt::Key_Hyper_L;
  case XKB_KEY_Hyper_R:
    return Qt::Key_Hyper_R;
  case XKB_KEY_Help:
    return Qt::Key_Help;
  case 0x1000FF74:
    return Qt::Key_Backtab;
  case 0x1005FF10:
    return Qt::Key_F11;
  case 0x1005FF11:
    return Qt::Key_F12;
  case XKB_KEY_KP_Space:
    return Qt::Key_Space;
  case XKB_KEY_KP_Tab:
    return Qt::Key_Tab;
  case XKB_KEY_KP_Enter:
    return Qt::Key_Enter;
  case XKB_KEY_KP_Home:
    return Qt::Key_Home;
  case XKB_KEY_KP_Left:
    return Qt::Key_Left;
  case XKB_KEY_KP_Up:
    return Qt::Key_Up;
  case XKB_KEY_KP_Right:
    return Qt::Key_Right;
  case XKB_KEY_KP_Down:
    return Qt::Key_Down;
  case XKB_KEY_KP_Prior:
    return Qt::Key_PageUp;
  case XKB_KEY_KP_Next:
    return Qt::Key_PageDown;
  case XKB_KEY_KP_End:
    return Qt::Key_End;
  case XKB_KEY_KP_Begin:
    return Qt::Key_Clear;
  case XKB_KEY_KP_Insert:
    return Qt::Key_Insert;
  case XKB_KEY_KP_Delete:
    return Qt::Key_Delete;
  case XKB_KEY_KP_Equal:
    return Qt::Key_Equal;
  case XKB_KEY_KP_Multiply:
    return Qt::Key_Asterisk;
  case XKB_KEY_KP_Add:
    return Qt::Key_Plus;
  case XKB_KEY_KP_Separator:
    return Qt::Key_Comma;
  case XKB_KEY_KP_Subtract:
    return Qt::Key_Minus;
  case XKB_KEY_KP_Decimal:
    return Qt::Key_Period;
  case XKB_KEY_KP_Divide:
    return Qt::Key_Slash;
  case XKB_KEY_Undo:
    return Qt::Key_Undo;
  case XKB_KEY_Redo:
    return Qt::Key_Redo;
  case XKB_KEY_Find:
    return Qt::Key_Find;
  case XKB_KEY_Cancel:
    return Qt::Key_Cancel;
  case XKB_KEY_ISO_Level3_Shift:
    return Qt::Key_AltGr;
  case XKB_KEY_Multi_key:
    return Qt::Key_Multi_key;
  case XKB_KEY_Codeinput:
    return Qt::Key_Codeinput;
  case XKB_KEY_SingleCandidate:
    return Qt::Key_SingleCandidate;
  case XKB_KEY_MultipleCandidate:
    return Qt::Key_MultipleCandidate;
  case XKB_KEY_PreviousCandidate:
    return Qt::Key_PreviousCandidate;
  case XKB_KEY_Mode_switch:
    return Qt::Key_Mode_switch;
  // case XKB_KEY_script_switch:
  //   return Qt::Key_Mode_switch;
  case XKB_KEY_Kanji:
    return Qt::Key_Kanji;
  case XKB_KEY_Muhenkan:
    return Qt::Key_Muhenkan;
  case XKB_KEY_Henkan_Mode:
    return Qt::Key_Henkan;
  // case XKB_KEY_Henkan:
  //   return Qt::Key_Henkan;
  case XKB_KEY_Romaji:
    return Qt::Key_Romaji;
  case XKB_KEY_Hiragana:
    return Qt::Key_Hiragana;
  case XKB_KEY_Katakana:
    return Qt::Key_Katakana;
  case XKB_KEY_Hiragana_Katakana:
    return Qt::Key_Hiragana_Katakana;
  case XKB_KEY_Zenkaku:
    return Qt::Key_Zenkaku;
  case XKB_KEY_Hankaku:
    return Qt::Key_Hankaku;
  case XKB_KEY_Zenkaku_Hankaku:
    return Qt::Key_Zenkaku_Hankaku;
  case XKB_KEY_Touroku:
    return Qt::Key_Touroku;
  case XKB_KEY_Massyo:
    return Qt::Key_Massyo;
  case XKB_KEY_Kana_Lock:
    return Qt::Key_Kana_Lock;
  case XKB_KEY_Kana_Shift:
    return Qt::Key_Kana_Shift;
  case XKB_KEY_Eisu_Shift:
    return Qt::Key_Eisu_Shift;
  case XKB_KEY_Eisu_toggle:
    return Qt::Key_Eisu_toggle;
  // case XKB_KEY_Kanji_Bangou:
  //   return Qt::Key_Codeinput;
  // case XKB_KEY_Zen_Koho:
  //   return Qt::Key_MultipleCandidate;
  // case XKB_KEY_Mae_Koho:
  //   return Qt::Key_PreviousCandidate;
  case XKB_KEY_Hangul:
    return Qt::Key_Hangul;
  case XKB_KEY_Hangul_Start:
    return Qt::Key_Hangul_Start;
  case XKB_KEY_Hangul_End:
    return Qt::Key_Hangul_End;
  case XKB_KEY_Hangul_Hanja:
    return Qt::Key_Hangul_Hanja;
  case XKB_KEY_Hangul_Jamo:
    return Qt::Key_Hangul_Jamo;
  case XKB_KEY_Hangul_Romaja:
    return Qt::Key_Hangul_Romaja;
  // case XKB_KEY_Hangul_Codeinput:
  //   return Qt::Key_Hangul_Codeinput;
  // case XKB_KEY_Hangul_Codeinput:
  //   return Qt::Key_Codeinput;
  case XKB_KEY_Hangul_Jeonja:
    return Qt::Key_Hangul_Jeonja;
  case XKB_KEY_Hangul_Banja:
    return Qt::Key_Hangul_Banja;
  case XKB_KEY_Hangul_PreHanja:
    return Qt::Key_Hangul_PreHanja;
  case XKB_KEY_Hangul_PostHanja:
    return Qt::Key_Hangul_PostHanja;
  // case XKB_KEY_Hangul_SingleCandidate:
  //   return Qt::Key_SingleCandidate;
  // case XKB_KEY_Hangul_MultipleCandidate:
  //   return Qt::Key_MultipleCandidate;
  // case XKB_KEY_Hangul_PreviousCandidate:
  //   return Qt::Key_PreviousCandidate;
  case XKB_KEY_Hangul_Special:
    return Qt::Key_Hangul_Special;
  // case XKB_KEY_Hangul_switch:
  //   return Qt::Key_Mode_switch;
  case XKB_KEY_dead_grave:
    return Qt::Key_Dead_Grave;
  case XKB_KEY_dead_acute:
    return Qt::Key_Dead_Acute;
  case XKB_KEY_dead_circumflex:
    return Qt::Key_Dead_Circumflex;
  case XKB_KEY_dead_tilde:
    return Qt::Key_Dead_Tilde;
  case XKB_KEY_dead_macron:
    return Qt::Key_Dead_Macron;
  case XKB_KEY_dead_breve:
    return Qt::Key_Dead_Breve;
  case XKB_KEY_dead_abovedot:
    return Qt::Key_Dead_Abovedot;
  case XKB_KEY_dead_diaeresis:
    return Qt::Key_Dead_Diaeresis;
  case XKB_KEY_dead_abovering:
    return Qt::Key_Dead_Abovering;
  case XKB_KEY_dead_doubleacute:
    return Qt::Key_Dead_Doubleacute;
  case XKB_KEY_dead_caron:
    return Qt::Key_Dead_Caron;
  case XKB_KEY_dead_cedilla:
    return Qt::Key_Dead_Cedilla;
  case XKB_KEY_dead_ogonek:
    return Qt::Key_Dead_Ogonek;
  case XKB_KEY_dead_iota:
    return Qt::Key_Dead_Iota;
  case XKB_KEY_dead_voiced_sound:
    return Qt::Key_Dead_Voiced_Sound;
  case XKB_KEY_dead_semivoiced_sound:
    return Qt::Key_Dead_Semivoiced_Sound;
  case XKB_KEY_dead_belowdot:
    return Qt::Key_Dead_Belowdot;
  case XKB_KEY_dead_hook:
    return Qt::Key_Dead_Hook;
  case XKB_KEY_dead_horn:
    return Qt::Key_Dead_Horn;
  case XKB_KEY_dead_stroke:
    return Qt::Key_Dead_Stroke;
  case XKB_KEY_dead_abovecomma:
    return Qt::Key_Dead_Abovecomma;
  case XKB_KEY_dead_abovereversedcomma:
    return Qt::Key_Dead_Abovereversedcomma;
  case XKB_KEY_dead_doublegrave:
    return Qt::Key_Dead_Doublegrave;
  case XKB_KEY_dead_belowring:
    return Qt::Key_Dead_Belowring;
  case XKB_KEY_dead_belowmacron:
    return Qt::Key_Dead_Belowmacron;
  case XKB_KEY_dead_belowcircumflex:
    return Qt::Key_Dead_Belowcircumflex;
  case XKB_KEY_dead_belowtilde:
    return Qt::Key_Dead_Belowtilde;
  case XKB_KEY_dead_belowbreve:
    return Qt::Key_Dead_Belowbreve;
  case XKB_KEY_dead_belowdiaeresis:
    return Qt::Key_Dead_Belowdiaeresis;
  case XKB_KEY_dead_invertedbreve:
    return Qt::Key_Dead_Invertedbreve;
  case XKB_KEY_dead_belowcomma:
    return Qt::Key_Dead_Belowcomma;
  case XKB_KEY_dead_currency:
    return Qt::Key_Dead_Currency;
  case XKB_KEY_dead_a:
    return Qt::Key_Dead_a;
  case XKB_KEY_dead_A:
    return Qt::Key_Dead_A;
  case XKB_KEY_dead_e:
    return Qt::Key_Dead_e;
  case XKB_KEY_dead_E:
    return Qt::Key_Dead_E;
  case XKB_KEY_dead_i:
    return Qt::Key_Dead_i;
  case XKB_KEY_dead_I:
    return Qt::Key_Dead_I;
  case XKB_KEY_dead_o:
    return Qt::Key_Dead_o;
  case XKB_KEY_dead_O:
    return Qt::Key_Dead_O;
  case XKB_KEY_dead_u:
    return Qt::Key_Dead_u;
  case XKB_KEY_dead_U:
    return Qt::Key_Dead_U;
  case XKB_KEY_dead_small_schwa:
    return Qt::Key_Dead_Small_Schwa;
  case XKB_KEY_dead_capital_schwa:
    return Qt::Key_Dead_Capital_Schwa;
  case XKB_KEY_dead_greek:
    return Qt::Key_Dead_Greek;
  case XKB_KEY_dead_lowline:
    return Qt::Key_Dead_Lowline;
  case XKB_KEY_dead_aboveverticalline:
    return Qt::Key_Dead_Aboveverticalline;
  case XKB_KEY_dead_belowverticalline:
    return Qt::Key_Dead_Belowverticalline;
  case XKB_KEY_dead_longsolidusoverlay:
    return Qt::Key_Dead_Longsolidusoverlay;
  case XKB_KEY_XF86Back:
    return Qt::Key_Back;
  case XKB_KEY_XF86Forward:
    return Qt::Key_Forward;
  case XKB_KEY_XF86Stop:
    return Qt::Key_Stop;
  case XKB_KEY_XF86Refresh:
    return Qt::Key_Refresh;
  case XKB_KEY_XF86Favorites:
    return Qt::Key_Favorites;
  case XKB_KEY_XF86AudioMedia:
    return Qt::Key_LaunchMedia;
  case XKB_KEY_XF86OpenURL:
    return Qt::Key_OpenUrl;
  case XKB_KEY_XF86HomePage:
    return Qt::Key_HomePage;
  case XKB_KEY_XF86Search:
    return Qt::Key_Search;
  case XKB_KEY_XF86AudioLowerVolume:
    return Qt::Key_VolumeDown;
  case XKB_KEY_XF86AudioMute:
    return Qt::Key_VolumeMute;
  case XKB_KEY_XF86AudioRaiseVolume:
    return Qt::Key_VolumeUp;
  case XKB_KEY_XF86AudioPlay:
    return Qt::Key_MediaPlay;
  case XKB_KEY_XF86AudioStop:
    return Qt::Key_MediaStop;
  case XKB_KEY_XF86AudioPrev:
    return Qt::Key_MediaPrevious;
  case XKB_KEY_XF86AudioNext:
    return Qt::Key_MediaNext;
  case XKB_KEY_XF86AudioRecord:
    return Qt::Key_MediaRecord;
  case XKB_KEY_XF86AudioPause:
    return Qt::Key_MediaPause;
  case XKB_KEY_XF86Mail:
    return Qt::Key_LaunchMail;
  case XKB_KEY_XF86MyComputer:
    return Qt::Key_LaunchMedia;
  case XKB_KEY_XF86Memo:
    return Qt::Key_Memo;
  case XKB_KEY_XF86ToDoList:
    return Qt::Key_ToDoList;
  case XKB_KEY_XF86Calendar:
    return Qt::Key_Calendar;
  case XKB_KEY_XF86PowerDown:
    return Qt::Key_PowerDown;
  case XKB_KEY_XF86ContrastAdjust:
    return Qt::Key_ContrastAdjust;
  case XKB_KEY_XF86Standby:
    return Qt::Key_Standby;
  case XKB_KEY_XF86MonBrightnessUp:
    return Qt::Key_MonBrightnessUp;
  case XKB_KEY_XF86MonBrightnessDown:
    return Qt::Key_MonBrightnessDown;
  case XKB_KEY_XF86KbdLightOnOff:
    return Qt::Key_KeyboardLightOnOff;
  case XKB_KEY_XF86KbdBrightnessUp:
    return Qt::Key_KeyboardBrightnessUp;
  case XKB_KEY_XF86KbdBrightnessDown:
    return Qt::Key_KeyboardBrightnessDown;
  case XKB_KEY_XF86PowerOff:
    return Qt::Key_PowerOff;
  case XKB_KEY_XF86WakeUp:
    return Qt::Key_WakeUp;
  case XKB_KEY_XF86Eject:
    return Qt::Key_Eject;
  case XKB_KEY_XF86ScreenSaver:
    return Qt::Key_ScreenSaver;
  case XKB_KEY_XF86WWW:
    return Qt::Key_WWW;
  case XKB_KEY_XF86Sleep:
    return Qt::Key_Sleep;
  case XKB_KEY_XF86LightBulb:
    return Qt::Key_LightBulb;
  case XKB_KEY_XF86Shop:
    return Qt::Key_Shop;
  case XKB_KEY_XF86History:
    return Qt::Key_History;
  case XKB_KEY_XF86AddFavorite:
    return Qt::Key_AddFavorite;
  case XKB_KEY_XF86HotLinks:
    return Qt::Key_HotLinks;
  case XKB_KEY_XF86BrightnessAdjust:
    return Qt::Key_BrightnessAdjust;
  case XKB_KEY_XF86Finance:
    return Qt::Key_Finance;
  case XKB_KEY_XF86Community:
    return Qt::Key_Community;
  case XKB_KEY_XF86AudioRewind:
    return Qt::Key_AudioRewind;
  case XKB_KEY_XF86BackForward:
    return Qt::Key_BackForward;
  case XKB_KEY_XF86ApplicationLeft:
    return Qt::Key_ApplicationLeft;
  case XKB_KEY_XF86ApplicationRight:
    return Qt::Key_ApplicationRight;
  case XKB_KEY_XF86Book:
    return Qt::Key_Book;
  case XKB_KEY_XF86CD:
    return Qt::Key_CD;
  case XKB_KEY_XF86Calculater:
    return Qt::Key_Calculator;
  case XKB_KEY_XF86Calculator:
    return Qt::Key_Calculator;
  case XKB_KEY_XF86Clear:
    return Qt::Key_Clear;
  case XKB_KEY_XF86ClearGrab:
    return Qt::Key_ClearGrab;
  case XKB_KEY_XF86Close:
    return Qt::Key_Close;
  case XKB_KEY_XF86Copy:
    return Qt::Key_Copy;
  case XKB_KEY_XF86Cut:
    return Qt::Key_Cut;
  case XKB_KEY_XF86Display:
    return Qt::Key_Display;
  case XKB_KEY_XF86DOS:
    return Qt::Key_DOS;
  case XKB_KEY_XF86Documents:
    return Qt::Key_Documents;
  case XKB_KEY_XF86Excel:
    return Qt::Key_Excel;
  case XKB_KEY_XF86Explorer:
    return Qt::Key_Explorer;
  case XKB_KEY_XF86Game:
    return Qt::Key_Game;
  case XKB_KEY_XF86Go:
    return Qt::Key_Go;
  case XKB_KEY_XF86iTouch:
    return Qt::Key_iTouch;
  case XKB_KEY_XF86LogOff:
    return Qt::Key_LogOff;
  case XKB_KEY_XF86Market:
    return Qt::Key_Market;
  case XKB_KEY_XF86Meeting:
    return Qt::Key_Meeting;
  case XKB_KEY_XF86MenuKB:
    return Qt::Key_MenuKB;
  case XKB_KEY_XF86MenuPB:
    return Qt::Key_MenuPB;
  case XKB_KEY_XF86MySites:
    return Qt::Key_MySites;
  case XKB_KEY_XF86New:
    return Qt::Key_New;
  case XKB_KEY_XF86News:
    return Qt::Key_News;
  case XKB_KEY_XF86OfficeHome:
    return Qt::Key_OfficeHome;
  case XKB_KEY_XF86Open:
    return Qt::Key_Open;
  case XKB_KEY_XF86Option:
    return Qt::Key_Option;
  case XKB_KEY_XF86Paste:
    return Qt::Key_Paste;
  case XKB_KEY_XF86Phone:
    return Qt::Key_Phone;
  case XKB_KEY_XF86PickupPhone:
    return Qt::Key_Call;
  case XKB_KEY_XF86HangupPhone:
    return Qt::Key_Hangup;
  case XKB_KEY_XF86Reply:
    return Qt::Key_Reply;
  case XKB_KEY_XF86Reload:
    return Qt::Key_Reload;
  case XKB_KEY_XF86RotateWindows:
    return Qt::Key_RotateWindows;
  case XKB_KEY_XF86RotationPB:
    return Qt::Key_RotationPB;
  case XKB_KEY_XF86RotationKB:
    return Qt::Key_RotationKB;
  case XKB_KEY_XF86Save:
    return Qt::Key_Save;
  case XKB_KEY_XF86Send:
    return Qt::Key_Send;
  case XKB_KEY_XF86Spell:
    return Qt::Key_Spell;
  case XKB_KEY_XF86SplitScreen:
    return Qt::Key_SplitScreen;
  case XKB_KEY_XF86Support:
    return Qt::Key_Support;
  case XKB_KEY_XF86TaskPane:
    return Qt::Key_TaskPane;
  case XKB_KEY_XF86Terminal:
    return Qt::Key_Terminal;
  case XKB_KEY_XF86Tools:
    return Qt::Key_Tools;
  case XKB_KEY_XF86Travel:
    return Qt::Key_Travel;
  case XKB_KEY_XF86Video:
    return Qt::Key_Video;
  case XKB_KEY_XF86Word:
    return Qt::Key_Word;
  case XKB_KEY_XF86Xfer:
    return Qt::Key_Xfer;
  case XKB_KEY_XF86ZoomIn:
    return Qt::Key_ZoomIn;
  case XKB_KEY_XF86ZoomOut:
    return Qt::Key_ZoomOut;
  case XKB_KEY_XF86Away:
    return Qt::Key_Away;
  case XKB_KEY_XF86Messenger:
    return Qt::Key_Messenger;
  case XKB_KEY_XF86WebCam:
    return Qt::Key_WebCam;
  case XKB_KEY_XF86MailForward:
    return Qt::Key_MailForward;
  case XKB_KEY_XF86Pictures:
    return Qt::Key_Pictures;
  case XKB_KEY_XF86Music:
    return Qt::Key_Music;
  case XKB_KEY_XF86Battery:
    return Qt::Key_Battery;
  // case XKB_KEY_XF86Bluetooth:
  //   return Qt::Key_Bluetooth;
  case XKB_KEY_XF86WLAN:
    return Qt::Key_WLAN;
  case XKB_KEY_XF86UWB:
    return Qt::Key_UWB;
  case XKB_KEY_XF86AudioForward:
    return Qt::Key_AudioForward;
  case XKB_KEY_XF86AudioRepeat:
    return Qt::Key_AudioRepeat;
  case XKB_KEY_XF86AudioRandomPlay:
    return Qt::Key_AudioRandomPlay;
  case XKB_KEY_XF86Subtitle:
    return Qt::Key_Subtitle;
  case XKB_KEY_XF86AudioCycleTrack:
    return Qt::Key_AudioCycleTrack;
  case XKB_KEY_XF86Time:
    return Qt::Key_Time;
  case XKB_KEY_XF86Select:
    return Qt::Key_Select;
  case XKB_KEY_XF86View:
    return Qt::Key_View;
  case XKB_KEY_XF86TopMenu:
    return Qt::Key_TopMenu;
  case XKB_KEY_XF86Red:
    return Qt::Key_Red;
  case XKB_KEY_XF86Green:
    return Qt::Key_Green;
  case XKB_KEY_XF86Yellow:
    return Qt::Key_Yellow;
  case XKB_KEY_XF86Blue:
    return Qt::Key_Blue;
  case XKB_KEY_XF86Bluetooth:
    return Qt::Key_Bluetooth;
  case XKB_KEY_XF86Suspend:
    return Qt::Key_Suspend;
  case XKB_KEY_XF86Hibernate:
    return Qt::Key_Hibernate;
  case XKB_KEY_XF86TouchpadToggle:
    return Qt::Key_TouchpadToggle;
  case XKB_KEY_XF86TouchpadOn:
    return Qt::Key_TouchpadOn;
  case XKB_KEY_XF86TouchpadOff:
    return Qt::Key_TouchpadOff;
  case XKB_KEY_XF86AudioMicMute:
    return Qt::Key_MicMute;
  case XKB_KEY_XF86Keyboard:
    return Qt::Key_Keyboard;
  case XKB_KEY_XF86Launch0:
    return Qt::Key_Launch0;
  case XKB_KEY_XF86Launch1:
    return Qt::Key_Launch1;
  case XKB_KEY_XF86Launch2:
    return Qt::Key_Launch2;
  case XKB_KEY_XF86Launch3:
    return Qt::Key_Launch3;
  case XKB_KEY_XF86Launch4:
    return Qt::Key_Launch4;
  case XKB_KEY_XF86Launch5:
    return Qt::Key_Launch5;
  case XKB_KEY_XF86Launch6:
    return Qt::Key_Launch6;
  case XKB_KEY_XF86Launch7:
    return Qt::Key_Launch7;
  case XKB_KEY_XF86Launch8:
    return Qt::Key_Launch8;
  case XKB_KEY_XF86Launch9:
    return Qt::Key_Launch9;
  case XKB_KEY_XF86LaunchA:
    return Qt::Key_LaunchA;
  case XKB_KEY_XF86LaunchB:
    return Qt::Key_LaunchB;
  case XKB_KEY_XF86LaunchC:
    return Qt::Key_LaunchC;
  case XKB_KEY_XF86LaunchD:
    return Qt::Key_LaunchD;
  case XKB_KEY_XF86LaunchE:
    return Qt::Key_LaunchE;
  case XKB_KEY_XF86LaunchF:
    return Qt::Key_LaunchF;
  default:
    return Qt::Key_unknown;
  }
}

static void qt_UCSConvertCase(uint32_t code, xkb_keysym_t *lower,
                              xkb_keysym_t *upper) {
  *lower = QChar::toLower(code);
  *upper = QChar::toUpper(code);
}

static void xkbcommon_XConvertCase(xkb_keysym_t sym, xkb_keysym_t *lower,
                                   xkb_keysym_t *upper) {
  /* Latin 1 keysym */
  if (sym < 0x100) {
    qt_UCSConvertCase(sym, lower, upper);
    return;
  }

  /* Unicode keysym */
  if ((sym & 0xff000000) == 0x01000000) {
    qt_UCSConvertCase((sym & 0x00ffffff), lower, upper);
    *upper |= 0x01000000;
    *lower |= 0x01000000;
    return;
  }

  /* Legacy keysym */

  *lower = sym;
  *upper = sym;

  switch (sym >> 8) {
  case 1: /* Latin 2 */
    /* Assume the KeySym is a legal value (ignore discontinuities) */
    if (sym == XKB_KEY_Aogonek)
      *lower = XKB_KEY_aogonek;
    else if (sym >= XKB_KEY_Lstroke && sym <= XKB_KEY_Sacute)
      *lower += (XKB_KEY_lstroke - XKB_KEY_Lstroke);
    else if (sym >= XKB_KEY_Scaron && sym <= XKB_KEY_Zacute)
      *lower += (XKB_KEY_scaron - XKB_KEY_Scaron);
    else if (sym >= XKB_KEY_Zcaron && sym <= XKB_KEY_Zabovedot)
      *lower += (XKB_KEY_zcaron - XKB_KEY_Zcaron);
    else if (sym == XKB_KEY_aogonek)
      *upper = XKB_KEY_Aogonek;
    else if (sym >= XKB_KEY_lstroke && sym <= XKB_KEY_sacute)
      *upper -= (XKB_KEY_lstroke - XKB_KEY_Lstroke);
    else if (sym >= XKB_KEY_scaron && sym <= XKB_KEY_zacute)
      *upper -= (XKB_KEY_scaron - XKB_KEY_Scaron);
    else if (sym >= XKB_KEY_zcaron && sym <= XKB_KEY_zabovedot)
      *upper -= (XKB_KEY_zcaron - XKB_KEY_Zcaron);
    else if (sym >= XKB_KEY_Racute && sym <= XKB_KEY_Tcedilla)
      *lower += (XKB_KEY_racute - XKB_KEY_Racute);
    else if (sym >= XKB_KEY_racute && sym <= XKB_KEY_tcedilla)
      *upper -= (XKB_KEY_racute - XKB_KEY_Racute);
    break;
  case 2: /* Latin 3 */
    /* Assume the KeySym is a legal value (ignore discontinuities) */
    if (sym >= XKB_KEY_Hstroke && sym <= XKB_KEY_Hcircumflex)
      *lower += (XKB_KEY_hstroke - XKB_KEY_Hstroke);
    else if (sym >= XKB_KEY_Gbreve && sym <= XKB_KEY_Jcircumflex)
      *lower += (XKB_KEY_gbreve - XKB_KEY_Gbreve);
    else if (sym >= XKB_KEY_hstroke && sym <= XKB_KEY_hcircumflex)
      *upper -= (XKB_KEY_hstroke - XKB_KEY_Hstroke);
    else if (sym >= XKB_KEY_gbreve && sym <= XKB_KEY_jcircumflex)
      *upper -= (XKB_KEY_gbreve - XKB_KEY_Gbreve);
    else if (sym >= XKB_KEY_Cabovedot && sym <= XKB_KEY_Scircumflex)
      *lower += (XKB_KEY_cabovedot - XKB_KEY_Cabovedot);
    else if (sym >= XKB_KEY_cabovedot && sym <= XKB_KEY_scircumflex)
      *upper -= (XKB_KEY_cabovedot - XKB_KEY_Cabovedot);
    break;
  case 3: /* Latin 4 */
    /* Assume the KeySym is a legal value (ignore discontinuities) */
    if (sym >= XKB_KEY_Rcedilla && sym <= XKB_KEY_Tslash)
      *lower += (XKB_KEY_rcedilla - XKB_KEY_Rcedilla);
    else if (sym >= XKB_KEY_rcedilla && sym <= XKB_KEY_tslash)
      *upper -= (XKB_KEY_rcedilla - XKB_KEY_Rcedilla);
    else if (sym == XKB_KEY_ENG)
      *lower = XKB_KEY_eng;
    else if (sym == XKB_KEY_eng)
      *upper = XKB_KEY_ENG;
    else if (sym >= XKB_KEY_Amacron && sym <= XKB_KEY_Umacron)
      *lower += (XKB_KEY_amacron - XKB_KEY_Amacron);
    else if (sym >= XKB_KEY_amacron && sym <= XKB_KEY_umacron)
      *upper -= (XKB_KEY_amacron - XKB_KEY_Amacron);
    break;
  case 6: /* Cyrillic */
    /* Assume the KeySym is a legal value (ignore discontinuities) */
    if (sym >= XKB_KEY_Serbian_DJE && sym <= XKB_KEY_Serbian_DZE)
      *lower -= (XKB_KEY_Serbian_DJE - XKB_KEY_Serbian_dje);
    else if (sym >= XKB_KEY_Serbian_dje && sym <= XKB_KEY_Serbian_dze)
      *upper += (XKB_KEY_Serbian_DJE - XKB_KEY_Serbian_dje);
    else if (sym >= XKB_KEY_Cyrillic_YU && sym <= XKB_KEY_Cyrillic_HARDSIGN)
      *lower -= (XKB_KEY_Cyrillic_YU - XKB_KEY_Cyrillic_yu);
    else if (sym >= XKB_KEY_Cyrillic_yu && sym <= XKB_KEY_Cyrillic_hardsign)
      *upper += (XKB_KEY_Cyrillic_YU - XKB_KEY_Cyrillic_yu);
    break;
  case 7: /* Greek */
    /* Assume the KeySym is a legal value (ignore discontinuities) */
    if (sym >= XKB_KEY_Greek_ALPHAaccent && sym <= XKB_KEY_Greek_OMEGAaccent)
      *lower += (XKB_KEY_Greek_alphaaccent - XKB_KEY_Greek_ALPHAaccent);
    else if (sym >= XKB_KEY_Greek_alphaaccent &&
             sym <= XKB_KEY_Greek_omegaaccent &&
             sym != XKB_KEY_Greek_iotaaccentdieresis &&
             sym != XKB_KEY_Greek_upsilonaccentdieresis)
      *upper -= (XKB_KEY_Greek_alphaaccent - XKB_KEY_Greek_ALPHAaccent);
    else if (sym >= XKB_KEY_Greek_ALPHA && sym <= XKB_KEY_Greek_OMEGA)
      *lower += (XKB_KEY_Greek_alpha - XKB_KEY_Greek_ALPHA);
    else if (sym >= XKB_KEY_Greek_alpha && sym <= XKB_KEY_Greek_omega &&
             sym != XKB_KEY_Greek_finalsmallsigma)
      *upper -= (XKB_KEY_Greek_alpha - XKB_KEY_Greek_ALPHA);
    break;
  case 0x13: /* Latin 9 */
    if (sym == XKB_KEY_OE)
      *lower = XKB_KEY_oe;
    else if (sym == XKB_KEY_oe)
      *upper = XKB_KEY_OE;
    else if (sym == XKB_KEY_Ydiaeresis)
      *lower = XKB_KEY_ydiaeresis;
    break;
  }
}

static bool isLatin1(xkb_keysym_t sym) { return sym >= 0x20 && sym <= 0xff; }

static xkb_keysym_t qxkbcommon_xkb_keysym_to_upper(xkb_keysym_t ks) {
  xkb_keysym_t lower, upper;

  xkbcommon_XConvertCase(ks, &lower, &upper);

  return upper;
}

Qt::Key keysym_to_QTKey(xkb_keysym_t keysym) {
  if (keysym >= XKB_KEY_F1 && keysym <= XKB_KEY_F35) {
    return static_cast<Qt::Key>(Qt::Key_F1 + (keysym - XKB_KEY_F1));
  } else if (keysym >= XKB_KEY_KP_0 && keysym <= XKB_KEY_KP_9) {
    return static_cast<Qt::Key>(Qt::Key_0 + (keysym - XKB_KEY_KP_0));
  } else if (isLatin1(keysym)) {
    Qt::Key qtKey =
        static_cast<Qt::Key>(qxkbcommon_xkb_keysym_to_upper(keysym));
    if (!isLatin1(qtKey)) {
      return static_cast<Qt::Key>(keysym);
    } else {
      return qtKey;
    }
  } else {
    Qt::Key key = keysym_map(keysym);
    if (key != Qt::Key_unknown) {
      return key;
    }
  }

  std::stringstream ss;
  ss << "0x" << std::hex << keysym;
  qWarning() << "Unknown keysym: " << ss.str();
  return Qt::Key_unknown;
}
