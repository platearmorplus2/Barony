// GameUI.cpp

#include "GameUI.hpp"
#include "Frame.hpp"
#include "Image.hpp"
#include "Field.hpp"
#include "Button.hpp"

#include "../main.hpp"
#include "../game.hpp"
#include "../menu.hpp"
#include "../interface/interface.hpp"
#include "../stat.hpp"
#include "../player.hpp"
#include "../draw.hpp"
#include "../items.hpp"
#include "../mod_tools.hpp"
#include "../input.hpp"

#include <assert.h>

bool newui = false;
int selectedCursorOpacity = 255;
int oldSelectedCursorOpacity = 255;
int hotbarSlotOpacity = 255;
int hotbarSelectedSlotOpacity = 255;

void createHPMPBars(const int player)
{
	auto& hud_t = players[player]->hud;
	const int barTotalHeight = 34;
	const int hpBarStartY = (hud_t.hudFrame->getSize().y + hud_t.hudFrame->getSize().h) - 106;
	const int mpBarStartY = hpBarStartY + barTotalHeight;
	const int barWidth = 276;
	const int barStartX = 14;
	{
		hud_t.hpFrame = hud_t.hudFrame->addFrame("hp bar");
		hud_t.hpFrame->setHollow(true);

		SDL_Rect pos{ barStartX, hpBarStartY, barWidth, barTotalHeight };
		hud_t.hpFrame->setSize(pos);

		auto fadeFrame = hud_t.hpFrame->addFrame("hp fade frame");
		fadeFrame->setSize(SDL_Rect{0, 0, barWidth, barTotalHeight});
		fadeFrame->setInheritParentFrameOpacity(false);

		auto foregroundFrame = hud_t.hpFrame->addFrame("hp foreground frame");
		foregroundFrame->setSize(SDL_Rect{ 0, 0, barWidth, barTotalHeight });


		auto base = hud_t.hpFrame->addImage(SDL_Rect{ 54, 4, barWidth - 54, 26 }, 0xFFFFFFFF,
			"images/ui/HUD/hpmpbars/HUD_Bars_Base_00.png", "hp img base");

		const int progressBarHeight = 22;
		auto fadeProgressBase = fadeFrame->addImage(SDL_Rect{ 54, 6, 6, progressBarHeight }, 0xFFFFFFFF,
			"images/ui/HUD/hpmpbars/HUD_Bars_HPMidFade_00.png", "hp img fade bot");
		auto fadeProgress = fadeFrame->addImage(SDL_Rect{ 60, 6, barWidth - 60 - 8,  progressBarHeight }, 0xFFFFFFFF,
			"images/ui/HUD/hpmpbars/HUD_Bars_HPMidFade_00.png", "hp img fade");
		auto fadeProgressEndCap = fadeFrame->addImage(SDL_Rect{
			fadeProgress->pos.x + fadeProgress->pos.w, 6, 8, progressBarHeight }, 0xFFFFFFFF,
			"images/ui/HUD/hpmpbars/HUD_Bars_HPEndFade_00.png", "hp img fade endcap");

		auto numbase = foregroundFrame->addImage(SDL_Rect{ 0, 4, 48, 26 }, 0xFFFFFFFF,
			"images/ui/HUD/hpmpbars/HUD_Bars_HPNumBase_00.png", "hp img value");
		auto div = foregroundFrame->addImage(SDL_Rect{ 46, 0, 8, 34 }, 0xFFFFFFFF,
			"images/ui/HUD/hpmpbars/HUD_Bars_Separator_00.png", "hp img div");

		auto currentProgressBase = foregroundFrame->addImage(SDL_Rect{ 54, 6, 6, progressBarHeight }, 0xFFFFFFFF,
			"images/ui/HUD/hpmpbars/HUD_Bars_HPBot_00.png", "hp img progress bot");
		auto currentProgress = foregroundFrame->addImage(SDL_Rect{ 60, 6, barWidth - 60 - 8,  progressBarHeight }, 0xFFFFFFFF,
			"images/ui/HUD/hpmpbars/HUD_Bars_HPMid_00.png", "hp img progress");
		auto currentProgressEndCap = foregroundFrame->addImage(SDL_Rect{
			currentProgress->pos.x + currentProgress->pos.w, 6, 8, progressBarHeight }, 0xFFFFFFFF,
			"images/ui/HUD/hpmpbars/HUD_Bars_HPEnd_00.png", "hp img progress endcap");

		const int endCapWidth = 16;
		auto endCap = foregroundFrame->addImage(SDL_Rect{ pos.w - endCapWidth, 0, endCapWidth, barTotalHeight }, 0xFFFFFFFF,
			"images/ui/HUD/hpmpbars/HUD_Bars_EndCap_00.png", "hp img endcap");

		auto font = "fonts/pixel_maz.ttf#16#2";
		auto hptext = foregroundFrame->addField("hp text", 16);
		hptext->setText("0");
		hptext->setSize(numbase->pos);
		hptext->setFont(font);
		hptext->setVJustify(Field::justify_t::CENTER);
		hptext->setHJustify(Field::justify_t::CENTER);
		hptext->setColor(SDL_MapRGBA(mainsurface->format, 255, 255, 255, 255));
	}

	// MP bar below
	{
		hud_t.mpFrame = hud_t.hudFrame->addFrame("mp bar");
		hud_t.mpFrame->setHollow(true);

		SDL_Rect pos{ barStartX, mpBarStartY, barWidth, barTotalHeight };
		hud_t.mpFrame->setSize(pos);

		auto fadeFrame = hud_t.mpFrame->addFrame("mp fade frame");
		fadeFrame->setSize(SDL_Rect{ 0, 0, barWidth, barTotalHeight });
		fadeFrame->setInheritParentFrameOpacity(false);

		auto foregroundFrame = hud_t.mpFrame->addFrame("mp foreground frame");
		foregroundFrame->setSize(SDL_Rect{ 0, 0, barWidth, barTotalHeight });


		auto base = hud_t.mpFrame->addImage(SDL_Rect{ 54, 4, barWidth - 54, 26 }, 0xFFFFFFFF,
			"images/ui/HUD/hpmpbars/HUD_Bars_Base_00.png", "mp img base");

		const int progressBarHeight = 22;
		auto fadeProgressBase = fadeFrame->addImage(SDL_Rect{ 54, 6, 6, progressBarHeight }, 0xFFFFFFFF,
			"images/ui/HUD/hpmpbars/HUD_Bars_MPMidFade_00.png", "mp img fade bot");
		auto fadeProgress = fadeFrame->addImage(SDL_Rect{ 60, 6, barWidth - 60 - 8,  progressBarHeight }, 0xFFFFFFFF,
			"images/ui/HUD/hpmpbars/HUD_Bars_MPMidFade_00.png", "mp img fade");
		auto fadeProgressEndCap = fadeFrame->addImage(SDL_Rect{
			fadeProgress->pos.x + fadeProgress->pos.w, 6, 8, progressBarHeight }, 0xFFFFFFFF,
			"images/ui/HUD/hpmpbars/HUD_Bars_MPEndFade_00.png", "mp img fade endcap");

		auto numbase = foregroundFrame->addImage(SDL_Rect{ 0, 4, 48, 26 }, 0xFFFFFFFF,
			"images/ui/HUD/hpmpbars/HUD_Bars_MPNumBase_00.png", "mp img value");
		auto div = foregroundFrame->addImage(SDL_Rect{ 46, 0, 8, 34 }, 0xFFFFFFFF,
			"images/ui/HUD/hpmpbars/HUD_Bars_Separator_00.png", "mp img div");

		auto currentProgressBase = foregroundFrame->addImage(SDL_Rect{ 54, 6, 6, progressBarHeight }, 0xFFFFFFFF,
			"images/ui/HUD/hpmpbars/HUD_Bars_MPBot_00.png", "mp img progress bot");
		auto currentProgress = foregroundFrame->addImage(SDL_Rect{ 60, 6, barWidth - 60 - 8,  progressBarHeight }, 0xFFFFFFFF,
			"images/ui/HUD/hpmpbars/HUD_Bars_MPMid_00.png", "mp img progress");
		auto currentProgressEndCap = foregroundFrame->addImage(SDL_Rect{
			currentProgress->pos.x + currentProgress->pos.w, 6, 8, progressBarHeight }, 0xFFFFFFFF,
			"images/ui/HUD/hpmpbars/HUD_Bars_MPEnd_00.png", "mp img progress endcap");

		const int endCapWidth = 16;
		auto endCap = foregroundFrame->addImage(SDL_Rect{ pos.w - endCapWidth, 0, endCapWidth, barTotalHeight }, 0xFFFFFFFF,
			"images/ui/HUD/hpmpbars/HUD_Bars_EndCap_00.png", "mp img endcap");

		auto font = "fonts/pixel_maz.ttf#16#2";
		auto mptext = foregroundFrame->addField("mp text", 16);
		mptext->setText("0");
		mptext->setSize(numbase->pos);
		mptext->setFont(font);
		mptext->setVJustify(Field::justify_t::CENTER);
		mptext->setHJustify(Field::justify_t::CENTER);
		mptext->setColor(SDL_MapRGBA(mainsurface->format, 255, 255, 255, 255));
	}
}

void createXPBar(const int player)
{
	auto& hud_t = players[player]->hud;
	hud_t.xpFrame = hud_t.hudFrame->addFrame("xp bar");
	hud_t.xpFrame->setHollow(true);

	const int xpBarStartY = (hud_t.hudFrame->getSize().y + hud_t.hudFrame->getSize().h) - 44;
	const int xpBarWidth = 650;
	const int xpBarTotalHeight = 34;
	SDL_Rect pos { (hud_t.hudFrame->getSize().x + hud_t.hudFrame->getSize().w / 2) - xpBarWidth / 2, xpBarStartY, xpBarWidth, xpBarTotalHeight };
	hud_t.xpFrame->setSize(pos);

	auto bg = hud_t.xpFrame->addImage(pos, 0xFFFFFFFF, "images/ui/HUD/xpbar/HUD_Bars_Base_00.png", "xp img base");
	bg->pos.x = 0;
	bg->pos.h = 26;
	bg->pos.y = 4;

	// xpProgress only adjusts width
	const int progressBarHeight = 22;
	auto xpProgress = hud_t.xpFrame->addImage(SDL_Rect{ 0, 6, 1, progressBarHeight }, 0xFFFFFFFF,
		"images/ui/HUD/xpbar/HUD_Bars_ExpMid_00.png", "xp img progress");

	// xpProgressEndCap only adjusts x position based on xpProgress->pos.x + xpProgress->pos.w
	auto xpProgressEndCap = hud_t.xpFrame->addImage(SDL_Rect{0, 6, 8, progressBarHeight }, 0xFFFFFFFF,
		"images/ui/HUD/xpbar/HUD_Bars_ExpEnd_00.png", "xp img progress endcap");

	const int endCapWidth = 26;
	SDL_Rect endCapPos {0, 0, endCapWidth, xpBarTotalHeight};
	auto endCapLeft = hud_t.xpFrame->addImage(endCapPos, 0xFFFFFFFF, "images/ui/HUD/xpbar/HUD_Bars_ExpCap1_00.png", "xp img endcap left");
	endCapPos.x = pos.w - endCapPos.w;
	auto endCapRight = hud_t.xpFrame->addImage(endCapPos, 0xFFFFFFFF, "images/ui/HUD/xpbar/HUD_Bars_ExpCap2_00.png", "xp img endcap right");

	const int textWidth = 40;
	auto font = "fonts/pixel_maz.ttf#16#2";
	auto textStatic = hud_t.xpFrame->addField("xp text static", 16);
	textStatic->setText("/100");
	textStatic->setSize(SDL_Rect{ pos.w / 2 - 4, 0, textWidth, pos.h }); // x - 4 to center the slash
	textStatic->setFont(font);
	textStatic->setVJustify(Field::justify_t::CENTER);
	textStatic->setHJustify(Field::justify_t::LEFT);
	textStatic->setColor(SDL_MapRGBA(mainsurface->format, 255, 255, 255, 255));

	auto text = hud_t.xpFrame->addField("xp text current", 16);
	text->setText("0");
	text->setSize(SDL_Rect{ pos.w / 2 - (4 * 2) - textWidth, 0, textWidth, pos.h }); // x - 4 to center the slash
	text->setFont(font);
	text->setVJustify(Field::justify_t::CENTER);
	text->setHJustify(Field::justify_t::RIGHT);
	text->setColor(SDL_MapRGBA(mainsurface->format, 255, 255, 255, 255));
}

void createHotbar(const int player)
{
	auto& hotbar_t = players[player]->hotbar;
	if ( !hotbar_t.hotbarFrame )
	{
		return;
	}
	Uint32 color = SDL_MapRGBA(mainsurface->format, 255, 255, 255, hotbarSlotOpacity);
	SDL_Rect slotPos{ 0, 0, hotbar_t.getSlotSize(), hotbar_t.getSlotSize() };
	for ( int i = 0; i < NUM_HOTBAR_SLOTS; ++i )
	{
		char slotname[32];
		snprintf(slotname, sizeof(slotname), "hotbar slot %d", i);
		auto slot = hotbar_t.hotbarFrame->addFrame(slotname);
		slot->setSize(slotPos);
		slot->addImage(slotPos, color, "images/ui/HUD/hotbar/HUD_Quickbar_Slot_Box_00.png", "slot img");

		char glyphname[32];
		snprintf(glyphname, sizeof(glyphname), "hotbar glyph %d", i);
		auto glyph = hotbar_t.hotbarFrame->addImage(slotPos, 0xFFFFFFFF, "images/ui/Glyphs/G_Switch_A00.png", glyphname);
		glyph->disabled = true;
	}

	auto font = "fonts/pixel_maz.ttf#18#2";

	for ( int i = 0; i < NUM_HOTBAR_SLOTS; ++i )
	{
		char slotname[32];
		snprintf(slotname, sizeof(slotname), "hotbar slot %d", i);
		auto slot = hotbar_t.hotbarFrame->findFrame(slotname);
		assert(slot);

		auto itemSlot = slot->addFrame("slot item");
		itemSlot->setSize(slot->getSize());
		createPlayerInventorySlotFrameElements(itemSlot);

		char numStr[4];
		snprintf(numStr, sizeof(numStr), "%d", i + 1);
		auto text = slot->addField("slot num text", 4);
		text->setText(numStr);
		text->setSize(SDL_Rect{ 0, -8, slotPos.w, slotPos.h });
		text->setFont(font);
		text->setVJustify(Field::justify_t::TOP);
		text->setHJustify(Field::justify_t::LEFT);
		text->setOntop(true);
	}

	auto highlightFrame = hotbar_t.hotbarFrame->addFrame("hotbar highlight");
	highlightFrame->setSize(slotPos);
	highlightFrame->addImage(slotPos, color, "images/ui/HUD/hotbar/HUD_Quickbar_Slot_HighlightBox_00.png", "highlight img");
	auto itemSlot = highlightFrame->addFrame("slot item");
	itemSlot->setSize(highlightFrame->getSize());
	createPlayerInventorySlotFrameElements(itemSlot);

	auto cursorFrame = hotbar_t.hotbarFrame->addFrame("shootmode selected item cursor");
	cursorFrame->setSize(SDL_Rect{ 0, 0, slotPos.w + 16, slotPos.h + 16 });
	cursorFrame->setDisabled(true);
	color = SDL_MapRGBA(mainsurface->format, 255, 255, 255, selectedCursorOpacity);
	cursorFrame->addImage(SDL_Rect{ 0, 0, 14, 14 },
		color, "images/ui/Inventory/Selector_TL.png", "shootmode selected cursor topleft");
	cursorFrame->addImage(SDL_Rect{ 0, 0, 14, 14 },
		color, "images/ui/Inventory/Selector_TR.png", "shootmode selected cursor topright");
	cursorFrame->addImage(SDL_Rect{ 0, 0, 14, 14 },
		color, "images/ui/Inventory/Selector_BL.png", "shootmode selected cursor bottomleft");
	cursorFrame->addImage(SDL_Rect{ 0, 0, 14, 14 },
		color, "images/ui/Inventory/Selector_BR.png", "shootmode selected cursor bottomright");

	auto text = highlightFrame->addField("slot num text", 4);
	text->setText("");
	text->setSize(SDL_Rect{ 0, -8, slotPos.w, slotPos.h });
	text->setFont(font);
	text->setVJustify(Field::justify_t::TOP);
	text->setHJustify(Field::justify_t::LEFT);
	text->setOntop(true);
}

void Player::HUD_t::processHUD()
{
	char name[32];
	snprintf(name, sizeof(name), "player hud %d", player.playernum);
	if ( !hudFrame )
	{
		hudFrame = gui->addFrame(name);
		hudFrame->setHollow(true);
		hudFrame->setBorder(0);
		hudFrame->setOwner(player.playernum);
	}
	hudFrame->setSize(SDL_Rect{ players[player.playernum]->camera_x1(),
		players[player.playernum]->camera_y1(),
		Frame::virtualScreenX,
		Frame::virtualScreenY });

	if ( nohud || !players[player.playernum]->isLocalPlayer() )
	{
		// hide
		hudFrame->setDisabled(true);
	}
	else
	{
		hudFrame->setDisabled(false);
	}

	if ( !xpFrame )
	{
		createXPBar(player.playernum);
	}
	if ( !hpFrame )
	{
		createHPMPBars(player.playernum);
	}
	updateXPBar();
	updateHPBar();
	updateMPBar();
}

void Player::Hotbar_t::processHotbar()
{
	char name[32];
	snprintf(name, sizeof(name), "player hotbar %d", player.playernum);
	if ( !hotbarFrame )
	{
		hotbarFrame = gui->addFrame(name);
		hotbarFrame->setHollow(true);
		hotbarFrame->setBorder(0);
		hotbarFrame->setOwner(player.playernum);
		createHotbar(player.playernum);
	}
	hotbarFrame->setSize(SDL_Rect{ players[player.playernum]->camera_x1(),
		players[player.playernum]->camera_y1(),
		players[player.playernum]->camera_width(),
		players[player.playernum]->camera_height() });

	if ( nohud || !players[player.playernum]->isLocalPlayer() )
	{
		// hide
		hotbarFrame->setDisabled(true);
	}
	else
	{
		hotbarFrame->setDisabled(false);
	}

	updateHotbar();
}

void createIngameHud(int player) {
    char name[32];
    snprintf(name, sizeof(name), "player hud %d", player);
    Frame* frame = gui->addFrame(name);

    players[player]->hud.hudFrame = frame;
    int playercount = 0;
    if (multiplayer == SINGLE) {
        for (int c = 0; c < MAXPLAYERS; ++c) {
            if (!client_disconnected[c]) {
                ++playercount;
            }
        }
    } else {
        playercount = 1;
    }

#ifdef NINTENDO
    static const char* bigfont = "rom://fonts/pixelmix.ttf#18";
    static const char* smallfont = "rom://fonts/pixel_maz.ttf#32";
#else
    static const char* bigfont = "fonts/pixelmix.ttf#18";
    static const char* smallfont = "fonts/pixel_maz.ttf#14";
#endif // NINTENDO

    // big empty frame to serve as the root
    if (playercount == 1) {
        frame->setSize(SDL_Rect{0, 0, Frame::virtualScreenX, Frame::virtualScreenY});
    } else if (playercount == 2) {
        int y = (player % 2) * Frame::virtualScreenY / 2;
        frame->setSize(SDL_Rect{0, y, Frame::virtualScreenX, Frame::virtualScreenY / 2});
    } else if (playercount >= 3 || playercount <= 4) {
        int x = (player % 2) * Frame::virtualScreenX / 2;
        int y = (player / 2) * Frame::virtualScreenY / 2;
        frame->setSize(SDL_Rect{x, y, Frame::virtualScreenX / 2, Frame::virtualScreenY / 2});
    }
    frame->setActualSize(SDL_Rect{0, 0, frame->getSize().w, frame->getSize().h});
    frame->setHollow(true);
    frame->setBorder(0);

    // chat
    Frame* chat = frame->addFrame("chat");
    chat->setSize(SDL_Rect{224, 16, 832, 200});
    chat->setActualSize(SDL_Rect{0, 0, 832, 200});
    chat->setFont(bigfont);
    {
        auto e = chat->addEntry("chat", true);
        e->color = 0xff00ffff;
        e->text = "Player 3: Chatlog text is so great what do you think?";
    }
    {
        auto e = chat->addEntry("message", true);
        e->color = 0xffffffff;
        e->text = "You have been hit by a dog.";
    }
    {
        auto e = chat->addEntry("message", true);
        e->color = 0xffffffff;
        e->text = "Yowza, dog hits smart!";
    }
    {
        auto e = chat->addEntry("message", true);
        e->color = 0xffffffff;
        e->text = "The dog hit smarts.";
    }
    {
        auto e = chat->addEntry("chat", true);
        e->color = 0xffff8800;
        e->text = "Player 2: Chatlog text is the best, I swear my life on it.";
    }

    // other players' statuses
    for (int i = 0, c = 0; c < MAXPLAYERS; ++c) {
        if (c == player) {
            continue;
        }

        // player name
        {
            char name[32];
            snprintf(name, sizeof(name), "Player %d Name", c + 1);
            Field* field = frame->addField(name, sizeof(name));
            Uint32 color = 0xffffffff;
            switch (c) {
            case 0: color = 0xffffffff; break;
            case 1: color = 0xffff8800; break;
            case 2: color = 0xff00ffff; break;
            case 3: color = 0xffff00ff; break;
            }
            field->setSize(SDL_Rect{16, 8 + 56 * i, 320, 64});
            field->setText(name);
            field->setFont(smallfont);
            field->setColor(color);
        }

        // hp bar
        {
            char name[32];
            snprintf(name, sizeof(name), "hp %d", c + 1);
            Frame* hp = frame->addFrame(name);
            hp->setColor(0xffffffff);
            hp->setBorderStyle(Frame::BORDER_BEVEL_HIGH);
            hp->setBorder(2);
            hp->setSize(SDL_Rect{16, 32 + 56 * i, 160, 16});
            hp->setActualSize(SDL_Rect{0, 0, hp->getSize().w, hp->getSize().h});
            auto red = hp->addImage(
                SDL_Rect{4, 4, hp->getSize().w - 8, hp->getSize().h - 8},
                0xff8888ff,
                "images/system/white.png",
                "red"
            );
        }

        // mp bar
        {
            char name[32];
            snprintf(name, sizeof(name), "mp %d", c + 1);
            Frame* mp = frame->addFrame(name);
            mp->setColor(0xffffffff);
            mp->setBorderStyle(Frame::BORDER_BEVEL_HIGH);
            mp->setBorder(2);
            mp->setSize(SDL_Rect{16, 48 + 56 * i, 160, 16});
            mp->setActualSize(SDL_Rect{0, 0, mp->getSize().w, mp->getSize().h});
            auto blue = mp->addImage(
                SDL_Rect{4, 4, mp->getSize().w - 8, mp->getSize().h - 8},
                0xffff8888,
                "images/system/white.png",
                "blue"
            );
        }

        // position
        ++i;
    }

    // ally hp
    for (int c = 0; c < 8; ++c) {
        // text
        {
            char text[32];
            snprintf(text, sizeof(text), "Ally %d", c + 1);
            Field* field = frame->addField(text, sizeof(text));
            field->setText(text);
            field->setSize(SDL_Rect{frame->getSize().w - 176, 8 + 18 * c, 64, 24});
            field->setFont(smallfont);
            field->setColor(0xff888888);
        }

        // bar
        {
            char name[32];
            snprintf(name, sizeof(name), "ally hp bar %d", c + 1);
            Frame* bar = frame->addFrame(name);
            bar->setSize(SDL_Rect{frame->getSize().w - 128, 14 + 18 * c, 112, 14});
            bar->setActualSize(SDL_Rect{0, 0, bar->getSize().w, bar->getSize().h});
            bar->setColor(0xffaaaaaa);
            auto red = bar->addImage(
                SDL_Rect{2, 2, bar->getSize().w - 4, bar->getSize().h - 4},
                0xff4444dd,
                "images/system/white.png",
                "red"
            );
        }
    }

    // hotbar
    static int num_hotbar_slots = 10;
    for (int c = 0; c < num_hotbar_slots; ++c) {
        Image* img = Image::get("images/system/hotbar_slot.png");
        Uint32 color = 0xffffffff;
        if (c == 2) {
            // selected one is highlighted yellow
            color = 0xff00ffff;
        }
        if (img) {
            int w = img->getWidth();
            int h = img->getHeight();
            int x = frame->getSize().w / 2 - w * num_hotbar_slots / 2 + w * c;
            int y = frame->getSize().h - h - 32;
            frame->addImage(
                SDL_Rect{x, y, w, h},
                color,
                "images/system/hotbar_slot.png",
                "hotbarslot"
            );
        }
    }

    // selected spell
    Image* spellbox_img = Image::get("images/system/hotbar_slot.png");
    if (spellbox_img) {
        int w = spellbox_img->getWidth();
        int h = spellbox_img->getHeight();
        int x = frame->getSize().w / 2 + w * num_hotbar_slots / 2 + 16;
        int y = frame->getSize().h - h - 32;
        frame->addImage(
            SDL_Rect{x, y, w, h},
            0xffffffff,
            "images/system/hotbar_slot.png",
            "spellbox"
        );
    }

    // xp bar
    if (spellbox_img) {
        int w = spellbox_img->getWidth();
        int h = spellbox_img->getHeight();

        Frame* xpbar = frame->addFrame("xpbar");
        xpbar->setSize(SDL_Rect{
            frame->getSize().w / 2 - w * num_hotbar_slots / 2,
            frame->getSize().h - 32,
            w * num_hotbar_slots,
            24
            }
        );
        xpbar->setActualSize(SDL_Rect{
            0,
            0,
            w * num_hotbar_slots,
            24
            }
        );
        xpbar->setColor(0xffaaaaaa);
        {
            auto progress = xpbar->addImage(
                SDL_Rect{2, 2, (xpbar->getSize().w - 4) / 4, xpbar->getSize().h - 4},
                0xffffffff,
                "images/system/white.png"
            );

            Field* text = xpbar->addField("text", 64);
            text->setSize(SDL_Rect{2, 4, xpbar->getSize().w - 4, xpbar->getSize().h - 4});
            text->setText("22 / 100 XP");
            text->setFont(smallfont);
            text->setColor(0xff888888);
            text->setHJustify(Field::justify_t::CENTER);
            text->setVJustify(Field::justify_t::CENTER);
        }
    }

    // hunger icon
    auto hunger = frame->addImage(
        SDL_Rect{16, frame->getSize().h - 160, 64, 64},
        0xffffffff,
        "images/system/Hunger.png",
        "hunger"
    );

    // hp
    {
        Frame* hp = frame->addFrame("hp");
        hp->setColor(0xffffffff);
        hp->setSize(SDL_Rect{16, frame->getSize().h - 96, 40, 24});
        hp->setActualSize(SDL_Rect{0, 0, 40, 24});
        {
            auto red = hp->addImage(
                SDL_Rect{4, 4, 32, 16},
                0xff8888ff,
                "images/system/white.png",
                "red"
            );
        }
        Frame* hp_bar = frame->addFrame("hp_bar");
        hp_bar->setColor(0xffffffff);
        hp_bar->setSize(SDL_Rect{56, frame->getSize().h - 96, 256, 24});
        hp_bar->setActualSize(SDL_Rect{0, 0, 256, 24});
        {
            auto red = hp_bar->addImage(
                SDL_Rect{4, 4, hp_bar->getSize().w - 8, 16},
                0xff8888ff,
                "images/system/white.png",
                "red"
            );
            Field* text = hp_bar->addField("text", 64);
            text->setSize(SDL_Rect{6, 3, 64, hp_bar->getSize().h - 4});
            text->setText("HP");
            text->setFont(smallfont);
            text->setColor(0xffffffff);
            text->setHJustify(Field::justify_t::LEFT);
            text->setVJustify(Field::justify_t::CENTER);
        }
    }

    // mp
    {
        Frame* mp = frame->addFrame("mp");
        mp->setColor(0xffffffff);
        mp->setSize(SDL_Rect{16, frame->getSize().h - 68, 40, 24});
        mp->setActualSize(SDL_Rect{0, 0, 40, 24});
        {
            auto blue = mp->addImage(
                SDL_Rect{4, 4, 32, 16},
                0xffff8888,
                "images/system/white.png",
                "blue"
            );
        }
        Frame* mp_bar = frame->addFrame("mp_bar");
        mp_bar->setColor(0xffffffff);
        mp_bar->setSize(SDL_Rect{56, frame->getSize().h - 68, 64, 24});
        mp_bar->setActualSize(SDL_Rect{0, 0, 64, 24});
        {
            auto blue = mp_bar->addImage(
                SDL_Rect{4, 4, mp_bar->getSize().w - 8, 16},
                0xffff8888,
                "images/system/white.png",
                "blue"
            );
            Field* text = mp_bar->addField("text", 64);
            text->setSize(SDL_Rect{6, 3, 64, mp_bar->getSize().h - 4});
            text->setText("MP");
            text->setFont(smallfont);
            text->setColor(0xffffffff);
            text->setHJustify(Field::justify_t::LEFT);
            text->setVJustify(Field::justify_t::CENTER);
        }
    }

    // status effect
    auto effect = frame->addImage(
        SDL_Rect{16, frame->getSize().h - 40, 32, 32},
        0xffffffff,
        "images/system/drunk.png",
        "status"
    );
}

void newIngameHud() {
    if (!nohud) {
        // here is where splitscreen
        if (!players[clientnum]->hud.hudFrame) {
            createIngameHud(clientnum);
        }

        // original minimap already works fine, so just reuse it
        if (multiplayer == SINGLE) {
            for (int c = 0; c < MAXPLAYERS; ++c) {
                if (!client_disconnected[c]) {
                    drawMinimap(c);
                }
            }
        } else {
            drawMinimap(0);
        }
    }
}

void createPlayerInventorySlotFrameElements(Frame* slotFrame)
{
	const SDL_Rect slotSize = SDL_Rect{ 0, 0, slotFrame->getSize().w, slotFrame->getSize().h };
	SDL_Rect coloredBackgroundPos = SDL_Rect{ slotSize.x + 2, slotSize.y + 2, slotSize.w - 2, slotSize.h - 2 };

	auto beatitudeFrame = slotFrame->addFrame("beatitude status frame"); // covers unidentified status as well
	beatitudeFrame->setSize(slotSize);
	beatitudeFrame->setActualSize(SDL_Rect{ 0, 0, slotSize.w, slotSize.h });
	beatitudeFrame->setHollow(true);
	beatitudeFrame->setDisabled(true);
	beatitudeFrame->addImage(coloredBackgroundPos, 0xFFFFFFFF, "images/system/white.png", "beatitude status bg");

	auto brokenStatusFrame = slotFrame->addFrame("broken status frame");
	brokenStatusFrame->setSize(slotSize);
	brokenStatusFrame->setActualSize(SDL_Rect{ 0, 0, slotSize.w, slotSize.h });
	brokenStatusFrame->setHollow(true);
	brokenStatusFrame->setDisabled(true);
	brokenStatusFrame->addImage(coloredBackgroundPos, SDL_MapRGBA(mainsurface->format, 160, 160, 160, 64), "images/system/white.png", "broken status bg");

	auto itemSpriteFrame = slotFrame->addFrame("item sprite frame");
	itemSpriteFrame->setSize(SDL_Rect{ slotSize.x + 3, slotSize.y + 3, slotSize.w - 3, slotSize.h - 3 });
	itemSpriteFrame->setActualSize(SDL_Rect{ slotSize.x + 3, slotSize.y + 3, slotSize.w - 3, slotSize.h - 3 });
	itemSpriteFrame->setHollow(true);
	itemSpriteFrame->setDisabled(true);
	itemSpriteFrame->addImage(slotSize, 0xFFFFFFFF, "images/system/white.png", "item sprite img");

	auto unusableFrame = slotFrame->addFrame("unusable item frame");
	unusableFrame->setSize(slotSize);
	unusableFrame->setActualSize(SDL_Rect{ 0, 0, slotSize.w, slotSize.h });
	unusableFrame->setHollow(true);
	unusableFrame->setDisabled(true);
	unusableFrame->addImage(coloredBackgroundPos, SDL_MapRGBA(mainsurface->format, 64, 64, 64, 144), "images/system/white.png", "unusable item bg");


	static const char* qtyfont = "fonts/pixel_maz.ttf#14#2";
	auto quantityFrame = slotFrame->addFrame("quantity frame");
	quantityFrame->setSize(slotSize);
	quantityFrame->setActualSize(SDL_Rect{ 0, 0, slotSize.w, slotSize.h });
	quantityFrame->setHollow(true);
	Field* qtyText = quantityFrame->addField("quantity text", 32);
	qtyText->setFont(qtyfont);
	qtyText->setColor(0xffffffff);
	qtyText->setHJustify(Field::justify_t::BOTTOM);
	qtyText->setVJustify(Field::justify_t::RIGHT);
	qtyText->setText("10");
	qtyText->setSize(SDL_Rect{ 0, 6, quantityFrame->getSize().w, quantityFrame->getSize().h });

	auto equippedIconFrame = slotFrame->addFrame("equipped icon frame");
	equippedIconFrame->setSize(slotSize);
	equippedIconFrame->setActualSize(SDL_Rect{ 0, 0, slotSize.w, slotSize.h });
	equippedIconFrame->setHollow(true);
	SDL_Rect equippedImgPos = { 3, slotSize.h - 17, 16, 16 };
	equippedIconFrame->addImage(equippedImgPos, 0xFFFFFFFF, "images/system/Equipped.png", "equipped icon img");

	auto brokenIconFrame = slotFrame->addFrame("broken icon frame");
	brokenIconFrame->setSize(slotSize);
	brokenIconFrame->setActualSize(SDL_Rect{ 0, 0, slotSize.w, slotSize.h });
	brokenIconFrame->setHollow(true);
	brokenIconFrame->addImage(equippedImgPos, 0xFFFFFFFF, "images/system/Broken.png", "broken icon img");
}

void resetInventorySlotFrames(const int player)
{
	char name[32];
	snprintf(name, sizeof(name), "player inventory %d", player);
	if ( Frame* inventoryFrame = gui->findFrame(name) )
	{
		for ( int x = 0; x < players[player]->inventoryUI.getSizeX(); ++x )
		{
			for ( int y = Player::Inventory_t::PaperDollRows::DOLL_ROW_1; y < players[player]->inventoryUI.getSizeY(); ++y )
			{
				if ( auto slotFrame = players[player]->inventoryUI.getInventorySlotFrame(x, y) )
				{
					slotFrame->setDisabled(true);
				}
			}
		}
	}
}

bool getSlotFrameXYFromMousePos(const int player, int& outx, int& outy)
{
	if ( !gui )
	{
		return false;
	}
	char name[32];
	snprintf(name, sizeof(name), "player inventory %d", player);
	if ( Frame* inventoryFrame = gui->findFrame(name) )
	{
		for ( int x = 0; x < players[player]->inventoryUI.getSizeX(); ++x )
		{
			for ( int y = Player::Inventory_t::DOLL_ROW_1; y < players[player]->inventoryUI.getSizeY(); ++y )
			{
				auto slotFrame = players[player]->inventoryUI.getInventorySlotFrame(x, y);
				if ( !slotFrame )
				{
					continue;
				}

				if ( slotFrame->capturesMouseInRealtimeCoords() )
				{
					outx = x;
					outy = y;
					return true;
				}
			}
		}
	}
	return false;
}

void updateSlotFrameFromItem(Frame* slotFrame, void* itemPtr)
{
	if ( !itemPtr || !slotFrame )
	{
		return;
	}

	Item* item = (Item*)itemPtr;

	int player = slotFrame->getOwner();

	slotFrame->setDisabled(false);

	auto spriteImageFrame = slotFrame->findFrame("item sprite frame");
	auto spriteImage = spriteImageFrame->findImage("item sprite img");
	if ( item->type == SPELL_ITEM )
	{
		spriteImage->path = ItemTooltips.getSpellIconPath(player, *item);
		spriteImageFrame->setDisabled(false);
	}
	else
	{
		node_t* imagePathsNode = list_Node(&items[item->type].images, item->appearance % items[item->type].variations);
		if ( imagePathsNode )
		{
			string_t* imagePath = static_cast<string_t*>(imagePathsNode->element);
			spriteImage->path = imagePath->data;
			spriteImageFrame->setDisabled(false);
		}
	}

	if ( auto qtyFrame = slotFrame->findFrame("quantity frame") )
	{
		qtyFrame->setDisabled(true);
		if ( item->count > 1 )
		{
			qtyFrame->setDisabled(false);
			if ( auto qtyText = qtyFrame->findField("quantity text") )
			{
				char qtybuf[32] = "";
				snprintf(qtybuf, sizeof(qtybuf), "%d", item->count);
				qtyText->setText(qtybuf);
			}
		}
	}

	bool disableBackgrounds = false;
	if ( !strcmp(slotFrame->getName(), "dragging inventory item") ) // dragging item, no need for colors
	{
		disableBackgrounds = true;
	}
	
	if ( auto beatitudeFrame = slotFrame->findFrame("beatitude status frame") )
	{
		beatitudeFrame->setDisabled(true);
		if ( !disableBackgrounds )
		{
			if ( auto beatitudeImg = beatitudeFrame->findImage("beatitude status bg") )
			{
				if ( !item->identified )
				{
					beatitudeImg->color = SDL_MapRGBA(mainsurface->format, 128, 128, 0, 125);
					beatitudeFrame->setDisabled(false);
				}
				else if ( item->beatitude < 0 )
				{
					beatitudeImg->color = SDL_MapRGBA(mainsurface->format, 128, 0, 0, 125);
					beatitudeFrame->setDisabled(false);
				}
				else if ( item->beatitude > 0 )
				{
					if ( colorblind )
					{
						beatitudeImg->color = SDL_MapRGBA(mainsurface->format, 100, 245, 255, 65);
					}
					else
					{
						beatitudeImg->color = SDL_MapRGBA(mainsurface->format, 0, 255, 0, 65);
					}
					beatitudeFrame->setDisabled(false);
				}
			}
		}
	}

	if ( auto brokenStatusFrame = slotFrame->findFrame("broken status frame") )
	{
		brokenStatusFrame->setDisabled(true);
		if ( !disableBackgrounds )
		{
			if ( item->status == BROKEN )
			{
				brokenStatusFrame->setDisabled(false);
			}
		}
	}

	if ( auto unusableFrame = slotFrame->findFrame("unusable item frame") )
	{
		bool greyedOut = false;
		unusableFrame->setDisabled(true);

		if ( !disableBackgrounds )
		{
			if ( players[player] && players[player]->entity && players[player]->entity->effectShapeshift != NOTHING )
			{
				// shape shifted, disable some items
				if ( !item->usableWhileShapeshifted(stats[player]) )
				{
					greyedOut = true;
				}
			}
			if ( !greyedOut && client_classes[player] == CLASS_SHAMAN
				&& item->type == SPELL_ITEM && !(playerUnlockedShamanSpell(player, item)) )
			{
				greyedOut = true;
			}

			if ( greyedOut )
			{
				unusableFrame->setDisabled(false);
			}
		}
	}

	bool equipped = false;
	bool broken = false;
	if ( itemCategory(item) != SPELL_CAT )
	{
		if ( itemIsEquipped(item, player) )
		{
			equipped = true;
		}
		else if ( item->status == BROKEN )
		{
			broken = true;
		}
	}
	else
	{
		spell_t* spell = getSpellFromItem(player, item);
		if ( players[player]->magic.selectedSpell() == spell
			&& (players[player]->magic.selected_spell_last_appearance == item->appearance || players[player]->magic.selected_spell_last_appearance == -1) )
		{
			equipped = true;
		}
	}

	if ( auto equippedIconFrame = slotFrame->findFrame("equipped icon frame") )
	{
		equippedIconFrame->setDisabled(true);
		if ( equipped && !disableBackgrounds )
		{
			equippedIconFrame->setDisabled(false);
		}
	}
	if ( auto brokenIconFrame = slotFrame->findFrame("broken icon frame") )
	{
		brokenIconFrame->setDisabled(true);
		if ( broken && !disableBackgrounds )
		{
			brokenIconFrame->setDisabled(false);
		}
	}
}

void createInventoryTooltipFrame(const int player)
{
	if ( !gui )
	{
		return;
	}

	char name[32];
	snprintf(name, sizeof(name), "player tooltip %d", player);

	if ( !players[player]->inventoryUI.tooltipFrame )
	{
		players[player]->inventoryUI.tooltipFrame = gui->addFrame(name);
		auto tooltipFrame = players[player]->inventoryUI.tooltipFrame;
		tooltipFrame->setSize(SDL_Rect{ 0, 0, 0, 0 });
		tooltipFrame->setActualSize(SDL_Rect{ 0, 0, tooltipFrame->getSize().w, tooltipFrame->getSize().h });
		tooltipFrame->setDisabled(true);
		tooltipFrame->setInheritParentFrameOpacity(false);
	}
	else
	{
		return;
	}

	auto tooltipFrame = players[player]->inventoryUI.tooltipFrame;

	Uint32 color = SDL_MapRGBA(mainsurface->format, 255, 255, 255, 255);
	tooltipFrame->addImage(SDL_Rect{ 0, 0, tooltipFrame->getSize().w, 28 },
		color, "images/ui/Inventory/tooltips/Hover_T00.png", "tooltip top background");
	tooltipFrame->addImage(SDL_Rect{ 0, 0, 16, 28 },
		color, "images/ui/Inventory/tooltips/Hover_TL00.png", "tooltip top left");
	tooltipFrame->addImage(SDL_Rect{ 0, 0, 16, 28 },
		color, "images/ui/Inventory/tooltips/Hover_TR00.png", "tooltip top right");

	tooltipFrame->addImage(SDL_Rect{ 0, 0, tooltipFrame->getSize().w, 52 },
		color, "images/ui/Inventory/tooltips/Hover_C00.png", "tooltip middle background");
	tooltipFrame->addImage(SDL_Rect{ 0, 0, 16, 52 },
		color, "images/ui/Inventory/tooltips/Hover_L00.png", "tooltip middle left");
	tooltipFrame->addImage(SDL_Rect{ 0, 0, 16, 52 },
		color, "images/ui/Inventory/tooltips/Hover_R00.png", "tooltip middle right");

	tooltipFrame->addImage(SDL_Rect{ 0, 0, tooltipFrame->getSize().w, 26 },
		color, "images/ui/Inventory/tooltips/Hover_B00.png", "tooltip bottom background");
	tooltipFrame->addImage(SDL_Rect{ 0, 0, 16, 26 },
		color, "images/ui/Inventory/tooltips/Hover_BL01.png", "tooltip bottom left");
	tooltipFrame->addImage(SDL_Rect{ 0, 0, 16, 26 },
		color, "images/ui/Inventory/tooltips/Hover_BR01.png", "tooltip bottom right");

	auto tooltipTextField = tooltipFrame->addField("inventory mouse tooltip header", 1024);
	tooltipTextField->setText("Nothing");
	tooltipTextField->setSize(SDL_Rect{ 0, 0, 0, 0 });
	tooltipTextField->setFont("fonts/pixelmix.ttf#14");
	tooltipTextField->setHJustify(Field::justify_t::LEFT);
	tooltipTextField->setVJustify(Field::justify_t::CENTER);
	tooltipTextField->setColor(SDL_MapRGBA(mainsurface->format, 67, 195, 157, 255));

	// temporary debug stuff
	{
		Frame::image_t* tmp = tooltipFrame->addImage(SDL_Rect{ 0, 0, 0, 0 },
		0xFFFFFFFF, "images/system/white.png", "inventory mouse tooltip min");
		tmp->color = SDL_MapRGBA(mainsurface->format, 255, 0, 0, 255);
		tmp->disabled = true;
		tmp = tooltipFrame->addImage(SDL_Rect{ 0, 0, 0, 0 },
			0xFFFFFFFF, "images/system/white.png", "inventory mouse tooltip max");
		tmp->color = SDL_MapRGBA(mainsurface->format, 0, 255, 0, 255);
		tmp->disabled = true;
		tmp = tooltipFrame->addImage(SDL_Rect{ 0, 0, 0, 0 },
			0xFFFFFFFF, "images/system/white.png", "inventory mouse tooltip header max");
		tmp->color = SDL_MapRGBA(mainsurface->format, 0, 255, 255, 255);
		tmp->disabled = true;
		tmp = tooltipFrame->addImage(SDL_Rect{ 0, 0, 0, 0 },
			0xFFFFFFFF, "images/system/white.png", "inventory mouse tooltip header bg");
		tmp->color = SDL_MapRGBA(mainsurface->format, 255, 255, 255, 255);
		tmp->disabled = true;
		tmp = tooltipFrame->addImage(SDL_Rect{ 0, 0, 0, 0 },
			0xFFFFFFFF, "images/system/white.png", "inventory mouse tooltip header bg new");
		tmp->color = SDL_MapRGBA(mainsurface->format, 255, 255, 0, 255);
		tmp->disabled = true;
	}

	if ( auto attrFrame = tooltipFrame->addFrame("inventory mouse tooltip attributes frame") )
	{
		attrFrame->setSize(SDL_Rect{ 0, 0, 0, 0 });

		auto spellImageBg = attrFrame->addImage(SDL_Rect{ 0, 0, 52, 52 },
			0xFFFFFFFF, "images/ui/Inventory/tooltips/SpellBorder_00.png", "inventory mouse tooltip spell image bg");
		spellImageBg->disabled = true;
		//spellImageBg->color = SDL_MapRGBA(mainsurface->format, 125, 125, 125, 228);
		auto spellImage = attrFrame->addImage(SDL_Rect{ 0, 0, 40, 40 },
			0xFFFFFFFF, "images/system/white.png", "inventory mouse tooltip spell image");
		spellImage->disabled = true;

		attrFrame->addImage(SDL_Rect{ 0, 0, 24, 24 },
			0xFFFFFFFF, "images/ui/Inventory/tooltips/HUD_Tooltip_Icon_Damage_00.png", "inventory mouse tooltip primary image");
		tooltipTextField = attrFrame->addField("inventory mouse tooltip primary value", 256);
		tooltipTextField->setText("Nothing");
		tooltipTextField->setSize(SDL_Rect{ 0, 0, 0, 0 });
		tooltipTextField->setFont("fonts/pixelmix.ttf#12");
		tooltipTextField->setHJustify(Field::justify_t::LEFT);
		tooltipTextField->setVJustify(Field::justify_t::CENTER);
		tooltipTextField->setColor(SDL_MapRGBA(mainsurface->format, 188, 154, 114, 255));

		tooltipTextField = attrFrame->addField("inventory mouse tooltip primary value highlight", 256);
		tooltipTextField->setText("Nothing");
		tooltipTextField->setSize(SDL_Rect{ 0, 0, 0, 0 });
		tooltipTextField->setFont("fonts/pixelmix.ttf#12");
		tooltipTextField->setHJustify(Field::justify_t::LEFT);
		tooltipTextField->setVJustify(Field::justify_t::CENTER);
		tooltipTextField->setColor(SDL_MapRGBA(mainsurface->format, 188, 154, 114, 255));

		tooltipTextField = attrFrame->addField("inventory mouse tooltip primary value positive text", 256);
		tooltipTextField->setText("Nothing");
		tooltipTextField->setSize(SDL_Rect{ 0, 0, 0, 0 });
		tooltipTextField->setFont("fonts/pixelmix.ttf#12");
		tooltipTextField->setHJustify(Field::justify_t::LEFT);
		tooltipTextField->setVJustify(Field::justify_t::CENTER);
		tooltipTextField->setColor(SDL_MapRGBA(mainsurface->format, 188, 154, 114, 255));

		tooltipTextField = attrFrame->addField("inventory mouse tooltip primary value negative text", 256);
		tooltipTextField->setText("Nothing");
		tooltipTextField->setSize(SDL_Rect{ 0, 0, 0, 0 });
		tooltipTextField->setFont("fonts/pixelmix.ttf#12");
		tooltipTextField->setHJustify(Field::justify_t::LEFT);
		tooltipTextField->setVJustify(Field::justify_t::CENTER);
		tooltipTextField->setColor(SDL_MapRGBA(mainsurface->format, 188, 154, 114, 255));

		tooltipTextField = attrFrame->addField("inventory mouse tooltip primary value slot name", 256);
		tooltipTextField->setText("Nothing");
		tooltipTextField->setSize(SDL_Rect{ 0, 0, 0, 0 });
		tooltipTextField->setFont("fonts/pixelmix.ttf#12");
		tooltipTextField->setHJustify(Field::justify_t::RIGHT);
		tooltipTextField->setVJustify(Field::justify_t::CENTER);
		tooltipTextField->setColor(SDL_MapRGBA(mainsurface->format, 188, 154, 114, 255));

		attrFrame->addImage(SDL_Rect{ 0, 0, 24, 24 },
			0xFFFFFFFF, "images/system/con32.png", "inventory mouse tooltip secondary image");
		tooltipTextField = attrFrame->addField("inventory mouse tooltip secondary value", 256);
		tooltipTextField->setText("Nothing");
		tooltipTextField->setSize(SDL_Rect{ 0, 0, 0, 0 });
		tooltipTextField->setFont("fonts/pixelmix.ttf#12");
		tooltipTextField->setHJustify(Field::justify_t::LEFT);
		tooltipTextField->setVJustify(Field::justify_t::CENTER);
		tooltipTextField->setColor(SDL_MapRGBA(mainsurface->format, 188, 154, 114, 255));

		tooltipTextField = attrFrame->addField("inventory mouse tooltip secondary value highlight", 256);
		tooltipTextField->setText("Nothing");
		tooltipTextField->setSize(SDL_Rect{ 0, 0, 0, 0 });
		tooltipTextField->setFont("fonts/pixelmix.ttf#12");
		tooltipTextField->setHJustify(Field::justify_t::LEFT);
		tooltipTextField->setVJustify(Field::justify_t::CENTER);
		tooltipTextField->setColor(SDL_MapRGBA(mainsurface->format, 188, 154, 114, 255));

		tooltipTextField = attrFrame->addField("inventory mouse tooltip secondary value positive text", 256);
		tooltipTextField->setText("Nothing");
		tooltipTextField->setSize(SDL_Rect{ 0, 0, 0, 0 });
		tooltipTextField->setFont("fonts/pixelmix.ttf#12");
		tooltipTextField->setHJustify(Field::justify_t::LEFT);
		tooltipTextField->setVJustify(Field::justify_t::CENTER);
		tooltipTextField->setColor(SDL_MapRGBA(mainsurface->format, 188, 154, 114, 255));

		tooltipTextField = attrFrame->addField("inventory mouse tooltip secondary value negative text", 256);
		tooltipTextField->setText("Nothing");
		tooltipTextField->setSize(SDL_Rect{ 0, 0, 0, 0 });
		tooltipTextField->setFont("fonts/pixelmix.ttf#12");
		tooltipTextField->setHJustify(Field::justify_t::LEFT);
		tooltipTextField->setVJustify(Field::justify_t::CENTER);
		tooltipTextField->setColor(SDL_MapRGBA(mainsurface->format, 188, 154, 114, 255));

		attrFrame->addImage(SDL_Rect{ 0, 0, 24, 24 },
			0xFFFFFFFF, "images/system/con32.png", "inventory mouse tooltip third image");
		tooltipTextField = attrFrame->addField("inventory mouse tooltip third value", 256);
		tooltipTextField->setText("Nothing");
		tooltipTextField->setSize(SDL_Rect{ 0, 0, 0, 0 });
		tooltipTextField->setFont("fonts/pixelmix.ttf#12");
		tooltipTextField->setHJustify(Field::justify_t::LEFT);
		tooltipTextField->setVJustify(Field::justify_t::CENTER);
		tooltipTextField->setColor(SDL_MapRGBA(mainsurface->format, 188, 154, 114, 255));

		tooltipTextField = attrFrame->addField("inventory mouse tooltip third value highlight", 256);
		tooltipTextField->setText("Nothing");
		tooltipTextField->setSize(SDL_Rect{ 0, 0, 0, 0 });
		tooltipTextField->setFont("fonts/pixelmix.ttf#12");
		tooltipTextField->setHJustify(Field::justify_t::LEFT);
		tooltipTextField->setVJustify(Field::justify_t::CENTER);
		tooltipTextField->setColor(SDL_MapRGBA(mainsurface->format, 188, 154, 114, 255));

		tooltipTextField = attrFrame->addField("inventory mouse tooltip third value positive text", 256);
		tooltipTextField->setText("Nothing");
		tooltipTextField->setSize(SDL_Rect{ 0, 0, 0, 0 });
		tooltipTextField->setFont("fonts/pixelmix.ttf#12");
		tooltipTextField->setHJustify(Field::justify_t::LEFT);
		tooltipTextField->setVJustify(Field::justify_t::CENTER);
		tooltipTextField->setColor(SDL_MapRGBA(mainsurface->format, 188, 154, 114, 255));

		tooltipTextField = attrFrame->addField("inventory mouse tooltip third value negative text", 256);
		tooltipTextField->setText("Nothing");
		tooltipTextField->setSize(SDL_Rect{ 0, 0, 0, 0 });
		tooltipTextField->setFont("fonts/pixelmix.ttf#12");
		tooltipTextField->setHJustify(Field::justify_t::LEFT);
		tooltipTextField->setVJustify(Field::justify_t::CENTER);
		tooltipTextField->setColor(SDL_MapRGBA(mainsurface->format, 188, 154, 114, 255));

		tooltipTextField = attrFrame->addField("inventory mouse tooltip attributes text", 1024);
		tooltipTextField->setText("Nothing");
		tooltipTextField->setSize(SDL_Rect{ 0, 0, 0, 0 });
		tooltipTextField->setFont("fonts/pixelmix.ttf#12");
		tooltipTextField->setHJustify(Field::justify_t::LEFT);
		tooltipTextField->setVJustify(Field::justify_t::TOP);
		tooltipTextField->setColor(SDL_MapRGBA(mainsurface->format, 188, 154, 114, 255));
	}
	if ( auto descFrame = tooltipFrame->addFrame("inventory mouse tooltip description frame") )
	{
		descFrame->setSize(SDL_Rect{ 0, 0, 0, 0 });

		descFrame->addImage(SDL_Rect{ 0, 0, 0, 1 },
			SDL_MapRGBA(mainsurface->format, 49, 53, 61, 255),
			"images/system/white.png", "inventory mouse tooltip description divider");

		tooltipTextField = descFrame->addField("inventory mouse tooltip description", 1024);
		tooltipTextField->setText("Nothing");
		tooltipTextField->setSize(SDL_Rect{ 0, 0, 0, 0 });
		tooltipTextField->setFont("fonts/pixelmix.ttf#12");
		tooltipTextField->setHJustify(Field::justify_t::LEFT);
		tooltipTextField->setVJustify(Field::justify_t::TOP);
		tooltipTextField->setColor(SDL_MapRGBA(mainsurface->format, 188, 154, 114, 255));
		tooltipTextField->setColor(SDL_MapRGBA(mainsurface->format, 67, 195, 157, 255));

		tooltipTextField = descFrame->addField("inventory mouse tooltip description positive text", 1024);
		tooltipTextField->setText("Nothing");
		tooltipTextField->setSize(SDL_Rect{ 0, 0, 0, 0 });
		tooltipTextField->setFont("fonts/pixelmix.ttf#12");
		tooltipTextField->setHJustify(Field::justify_t::LEFT);
		tooltipTextField->setVJustify(Field::justify_t::TOP);
		//tooltipTextField->setColor(SDL_MapRGBA(mainsurface->format, 1, 151, 246, 255));
		tooltipTextField->setColor(SDL_MapRGBA(mainsurface->format, 188, 154, 114, 255));

		tooltipTextField = descFrame->addField("inventory mouse tooltip description negative text", 1024);
		tooltipTextField->setText("Nothing");
		tooltipTextField->setSize(SDL_Rect{ 0, 0, 0, 0 });
		tooltipTextField->setFont("fonts/pixelmix.ttf#12");
		tooltipTextField->setHJustify(Field::justify_t::LEFT);
		tooltipTextField->setVJustify(Field::justify_t::TOP);
		tooltipTextField->setColor(SDL_MapRGBA(mainsurface->format, 215, 38, 61, 255));
	}
	if ( auto valueFrame = tooltipFrame->addFrame("inventory mouse tooltip value frame") )
	{
		valueFrame->setSize(SDL_Rect{ 0, 0, 0, 0 });

		valueFrame->addImage(SDL_Rect{ 0, 0, 0, 0 },
			SDL_MapRGBA(mainsurface->format, 49, 53, 61, 255), 
			"images/system/white.png", "inventory mouse tooltip value background");

		valueFrame->addImage(SDL_Rect{ 0, 0, 0, 1 },
			SDL_MapRGBA(mainsurface->format, 49, 53, 61, 255),
			"images/system/white.png", "inventory mouse tooltip value divider");

		tooltipTextField = valueFrame->addField("inventory mouse tooltip identified value", 64);
		tooltipTextField->setText("Nothing");
		tooltipTextField->setSize(SDL_Rect{ 0, 0, 0, 0 });
		tooltipTextField->setFont("fonts/pixelmix.ttf#12");
		tooltipTextField->setHJustify(Field::justify_t::LEFT);
		tooltipTextField->setVJustify(Field::justify_t::CENTER);
		tooltipTextField->setColor(SDL_MapRGBA(mainsurface->format, 188, 154, 114, 255));

		valueFrame->addImage(SDL_Rect{ 0, 0, 16, 16 },
			0xFFFFFFFF, "images/system/per32.png", "inventory mouse tooltip gold image");

		tooltipTextField = valueFrame->addField("inventory mouse tooltip gold value", 64);
		tooltipTextField->setText("Nothing");
		tooltipTextField->setSize(SDL_Rect{ 0, 0, 0, 0 });
		tooltipTextField->setFont("fonts/pixelmix.ttf#12");
		tooltipTextField->setHJustify(Field::justify_t::LEFT);
		tooltipTextField->setVJustify(Field::justify_t::CENTER);
		tooltipTextField->setColor(SDL_MapRGBA(mainsurface->format, 188, 154, 114, 255));

		valueFrame->addImage(SDL_Rect{ 0, 0, 16, 16 },
			0xFFFFFFFF, "images/system/int32.png", "inventory mouse tooltip weight image");

		tooltipTextField = valueFrame->addField("inventory mouse tooltip weight value", 64);
		tooltipTextField->setText("Nothing");
		tooltipTextField->setSize(SDL_Rect{ 0, 0, 0, 0 });
		tooltipTextField->setFont("fonts/pixelmix.ttf#12");
		tooltipTextField->setHJustify(Field::justify_t::LEFT);
		tooltipTextField->setVJustify(Field::justify_t::CENTER);
		tooltipTextField->setColor(SDL_MapRGBA(mainsurface->format, 188, 154, 114, 255));
	}
	if ( auto promptFrame = tooltipFrame->addFrame("inventory mouse tooltip prompt frame") )
	{
		promptFrame->setSize(SDL_Rect{ 0, 0, 0, 0 });

		tooltipTextField = promptFrame->addField("inventory mouse tooltip prompt", 1024);
		tooltipTextField->setText("Nothing");
		tooltipTextField->setSize(SDL_Rect{ 0, 0, 0, 0 });
		tooltipTextField->setFont("fonts/pixelmix.ttf#12");
		tooltipTextField->setHJustify(Field::justify_t::RIGHT);
		tooltipTextField->setVJustify(Field::justify_t::CENTER);
		tooltipTextField->setColor(SDL_MapRGBA(mainsurface->format, 148, 82, 3, 255));
	}
}

void drawCharacterPreview(const int player, SDL_Rect pos)
{
	double ofov = fov;
	fov = 50;

	//TempTexture* minimapTexture = new TempTexture();

	if ( players[player] != nullptr && players[player]->entity != nullptr )
	{
		if ( !softwaremode )
		{
			glClear(GL_DEPTH_BUFFER_BIT);
		}
		//TODO: These two NOT PLAYERSWAP
		//camera.x=players[player]->x/16.0+.5*cos(players[player]->yaw)-.4*sin(players[player]->yaw);
		//camera.y=players[player]->y/16.0+.5*sin(players[player]->yaw)+.4*cos(players[player]->yaw);
		camera_charsheet.x = players[player]->entity->x / 16.0 + (.92 * cos(camera_charsheet_offsetyaw));
		camera_charsheet.y = players[player]->entity->y / 16.0 + (.92 * sin(camera_charsheet_offsetyaw));
		camera_charsheet.z = players[player]->entity->z * 2;
		//camera.ang=atan2(players[player]->y/16.0-camera.y,players[player]->x/16.0-camera.x); //TODO: _NOT_ PLAYERSWAP
		camera_charsheet.ang = (camera_charsheet_offsetyaw - PI); //5 * PI / 4;
		camera_charsheet.vang = PI / 20;

		camera_charsheet.winx = pos.x;
		camera_charsheet.winy = pos.y;
		//camera_charsheet.winx = x1 + 8;
		//camera_charsheet.winy = y1 + 8;

		camera_charsheet.winw = pos.w;
		camera_charsheet.winh = pos.h;
		bool b = players[player]->entity->flags[BRIGHT];
		players[player]->entity->flags[BRIGHT] = true;
		if ( !players[player]->entity->flags[INVISIBLE] )
		{
			glDrawVoxel(&camera_charsheet, players[player]->entity, REALCOLORS);
		}
		players[player]->entity->flags[BRIGHT] = b;
		int c = 0;
		if ( multiplayer != CLIENT )
		{
			for ( node_t* node = players[player]->entity->children.first; node != nullptr; node = node->next )
			{
				if ( c == 0 )
				{
					c++;
					continue;
				}
				Entity* entity = (Entity*)node->element;
				if ( !entity->flags[INVISIBLE] )
				{
					b = entity->flags[BRIGHT];
					entity->flags[BRIGHT] = true;
					glDrawVoxel(&camera_charsheet, entity, REALCOLORS);
					entity->flags[BRIGHT] = b;
				}
				c++;
			}
			for ( node_t* node = map.entities->first; node != NULL; node = node->next )
			{
				Entity* entity = (Entity*)node->element;
				if ( (Sint32)entity->getUID() == -4 )
				{
					glDrawSprite(&camera_charsheet, entity, REALCOLORS);
				}
			}
		}
		else
		{
			for ( node_t* node = map.entities->first; node != NULL; node = node->next )
			{
				Entity* entity = (Entity*)node->element;
				if ( (entity->behavior == &actPlayerLimb && entity->skill[2] == player && !entity->flags[INVISIBLE]) || (Sint32)entity->getUID() == -4 )
				{
					b = entity->flags[BRIGHT];
					entity->flags[BRIGHT] = true;
					if ( (Sint32)entity->getUID() == -4 )
					{
						glDrawSprite(&camera_charsheet, entity, REALCOLORS);
					}
					else
					{
						glDrawVoxel(&camera_charsheet, entity, REALCOLORS);
					}
					entity->flags[BRIGHT] = b;
				}
			}
		}

		if ( Input::inputs[player].analog("InventoryCharacterRotateLeft") )
		{
			camera_charsheet_offsetyaw -= 0.05;
		}
		else if ( Input::inputs[player].analog("InventoryCharacterRotateRight") )
		{
			camera_charsheet_offsetyaw += 0.05;
		}

		if ( camera_charsheet_offsetyaw > 2 * PI )
		{
			camera_charsheet_offsetyaw -= 2 * PI;
		}
		if ( camera_charsheet_offsetyaw < 0.0 )
		{
			camera_charsheet_offsetyaw += 2 * PI;
		}
	}
	fov = ofov;
}

void loadHUDSettingsJSON()
{
	if ( !PHYSFS_getRealDir("/data/HUD_settings.json") )
	{
		printlog("[JSON]: Error: Could not find file: data/HUD_settings.json");
	}
	else
	{
		std::string inputPath = PHYSFS_getRealDir("/data/HUD_settings.json");
		inputPath.append("/data/HUD_settings.json");

		File* fp = FileIO::open(inputPath.c_str(), "rb");
		if ( !fp )
		{
			printlog("[JSON]: Error: Could not open json file %s", inputPath.c_str());
		}
		else
		{
			char buf[65536];
			int count = fp->read(buf, sizeof(buf[0]), sizeof(buf));
			buf[count] = '\0';
			rapidjson::StringStream is(buf);
			FileIO::close(fp);

			rapidjson::Document d;
			d.ParseStream(is);
			if ( !d.HasMember("version") )
			{
				printlog("[JSON]: Error: No 'version' value in json file, or JSON syntax incorrect! %s", inputPath.c_str());
			}
			else
			{
				if ( d.HasMember("hotbar_slot_opacity") )
				{
					hotbarSlotOpacity = d["hotbar_slot_opacity"].GetInt();
				}
				if ( d.HasMember("hotbar_selected_slot_opacity") )
				{
					hotbarSelectedSlotOpacity = d["hotbar_selected_slot_opacity"].GetInt();
				}
				if ( d.HasMember("selected_cursor_opacity") )
				{
					selectedCursorOpacity = d["selected_cursor_opacity"].GetInt();
				}
				if ( d.HasMember("selected_old_cursor_opacity") )
				{
					oldSelectedCursorOpacity = d["selected_old_cursor_opacity"].GetInt();
				}
				printlog("[JSON]: Successfully read json file %s", inputPath.c_str());
			}
		}
	}
}

void createPlayerInventory(const int player)
{
	char name[32];
	snprintf(name, sizeof(name), "player inventory %d", player);
	Frame* frame = gui->addFrame(name);
	players[player]->inventoryUI.frame = frame;
	frame->setSize(SDL_Rect{ players[player]->camera_x1(),
		players[player]->camera_y1(),
		players[player]->camera_width(),
		players[player]->camera_height() });
	frame->setActualSize(frame->getSize());
	frame->setHollow(true);
	frame->setBorder(0);
	frame->setOwner(player);
	frame->setInheritParentFrameOpacity(false);

	createInventoryTooltipFrame(player);

	SDL_Rect basePos{ 0, 0, 105 * 2, 224 * 2 };
	{
		auto bgFrame = frame->addFrame("inventory base");
		bgFrame->setSize(basePos);
		const auto bgSize = bgFrame->getSize();
		bgFrame->setActualSize(SDL_Rect{ 0, 0, bgSize.w, bgSize.h });
		bgFrame->addImage(SDL_Rect{ 0, 0, bgSize.w, bgSize.h },
			SDL_MapRGBA(mainsurface->format, 255, 255, 255, 255),
			"images/ui/Inventory/HUD_Inventory_Base_02.png", "inventory base img");
	}

	const int inventorySlotSize = players[player]->inventoryUI.getSlotSize();

	players[player]->inventoryUI.slotFrames.clear();

	const int baseSlotOffsetX = 4;
	const int baseSlotOffsetY = 0;
	SDL_Rect invSlotsPos{ 0, 202, basePos.w, 242 };
	{
		const auto invSlotsFrame = frame->addFrame("inventory slots");
		invSlotsFrame->setSize(invSlotsPos);
		invSlotsFrame->setActualSize(SDL_Rect{ 0, 0, invSlotsFrame->getSize().w, invSlotsFrame->getSize().h });

		SDL_Rect currentSlotPos{ baseSlotOffsetX, baseSlotOffsetY, inventorySlotSize, inventorySlotSize };

		for ( int x = 0; x < players[player]->inventoryUI.getSizeX(); ++x )
		{
			currentSlotPos.x = baseSlotOffsetX + (x * inventorySlotSize);
			for ( int y = 0; y < players[player]->inventoryUI.getSizeY(); ++y )
			{
				currentSlotPos.y = baseSlotOffsetY + (y * inventorySlotSize);

				char slotname[32] = "";
				snprintf(slotname, sizeof(slotname), "slot %d %d", x, y);

				auto slotFrame = invSlotsFrame->addFrame(slotname);
				players[player]->inventoryUI.slotFrames[x + y * 100] = slotFrame;
				SDL_Rect slotPos{ currentSlotPos.x, currentSlotPos.y, inventorySlotSize, inventorySlotSize };
				slotFrame->setSize(slotPos);
				slotFrame->setActualSize(SDL_Rect{ 0, 0, slotFrame->getSize().w, slotFrame->getSize().h });
				//slotFrame->setDisabled(true);

				createPlayerInventorySlotFrameElements(slotFrame);
			}
		}
	}

	{
		SDL_Rect dollSlotsPos{ 0, 0, basePos.w, invSlotsPos.y };
		const auto dollSlotsFrame = frame->addFrame("paperdoll slots");
		dollSlotsFrame->setSize(dollSlotsPos);
		dollSlotsFrame->setActualSize(SDL_Rect{ 0, 0, dollSlotsFrame->getSize().w, dollSlotsFrame->getSize().h });

		SDL_Rect currentSlotPos{ baseSlotOffsetX, baseSlotOffsetY, inventorySlotSize, inventorySlotSize };

		for ( int x = Player::Inventory_t::PaperDollColumns::DOLL_COLUMN_LEFT; x <= Player::Inventory_t::PaperDollColumns::DOLL_COLUMN_RIGHT; ++x )
		{
			currentSlotPos.x = baseSlotOffsetX;
			if ( x == Player::Inventory_t::PaperDollColumns::DOLL_COLUMN_RIGHT )
			{
				currentSlotPos.x = baseSlotOffsetX + (4 * inventorySlotSize); // 4 slots over
			}

			for ( int y = Player::Inventory_t::PaperDollRows::DOLL_ROW_1; y <= Player::Inventory_t::PaperDollRows::DOLL_ROW_5; ++y )
			{
				currentSlotPos.y = baseSlotOffsetY + ((y - Player::Inventory_t::PaperDollRows::DOLL_ROW_1) * inventorySlotSize);

				char slotname[32] = "";
				snprintf(slotname, sizeof(slotname), "slot %d %d", x, y);

				auto slotFrame = dollSlotsFrame->addFrame(slotname);
				players[player]->inventoryUI.slotFrames[x + y * 100] = slotFrame;
				SDL_Rect slotPos{ currentSlotPos.x, currentSlotPos.y, inventorySlotSize, inventorySlotSize };
				slotFrame->setSize(slotPos);
				slotFrame->setActualSize(SDL_Rect{ 0, 0, slotFrame->getSize().w, slotFrame->getSize().h });
				//slotFrame->setDisabled(true);

				createPlayerInventorySlotFrameElements(slotFrame);
			}
		}

		{
			auto charFrame = frame->addFrame("inventory character preview");
			auto charSize = dollSlotsPos;
			charSize.x += inventorySlotSize + baseSlotOffsetX + 4;
			charSize.w -= 2 * (inventorySlotSize + baseSlotOffsetX + 4);

			charFrame->setSize(charSize);
			charFrame->setDrawCallback([](Widget& widget, SDL_Rect pos) {
				drawCharacterPreview(widget.getOwner(), pos);
			});
			//charFrame->addImage(SDL_Rect{ 0, 0, charSize.w, charSize.h },
			//	SDL_MapRGBA(mainsurface->format, 255, 255, 255, 255),
			//	"images/system/white.png", "inventory character preview bg");
		}

		/*auto selectedFrame = dollSlotsFrame->addFrame("paperdoll selected item");
		selectedFrame->setSize(SDL_Rect{ 0, 0, inventorySlotSize, inventorySlotSize });
		selectedFrame->setActualSize(SDL_Rect{ 0, 0, selectedFrame->getSize().w, selectedFrame->getSize().h });
		selectedFrame->setDisabled(true);

		Uint32 color = SDL_MapRGBA(mainsurface->format, 255, 255, 0, 255);
		selectedFrame->addImage(SDL_Rect{ 0, 0, selectedFrame->getSize().w, selectedFrame->getSize().h },
			color, "images/system/hotbar_slot.png", "paperdoll selected highlight");*/
	}

	{
		auto selectedFrame = frame->addFrame("inventory selected item");
		selectedFrame->setSize(SDL_Rect{ 0, 0, inventorySlotSize, inventorySlotSize });
		selectedFrame->setDisabled(true);

		Uint32 color = SDL_MapRGBA(mainsurface->format, 255, 255, 0, 255);
		selectedFrame->addImage(SDL_Rect{ 0, 0, selectedFrame->getSize().w, selectedFrame->getSize().h },
			color, "images/system/hotbar_slot.png", "inventory selected highlight");

		auto oldSelectedFrame = frame->addFrame("inventory old selected item");
		oldSelectedFrame->setSize(SDL_Rect{ 0, 0, inventorySlotSize, inventorySlotSize });
		oldSelectedFrame->setDisabled(true);

		color = SDL_MapRGBA(mainsurface->format, 0, 255, 255, 255);
		auto oldImg = oldSelectedFrame->addImage(SDL_Rect{ 0, 0, oldSelectedFrame->getSize().w, oldSelectedFrame->getSize().h },
			SDL_MapRGBA(mainsurface->format, 255, 255, 255, 128), "", "inventory old selected item");
		oldImg->disabled = true;
		oldSelectedFrame->addImage(SDL_Rect{ 0, 0, oldSelectedFrame->getSize().w, oldSelectedFrame->getSize().h },
			color, "images/system/hotbar_slot.png", "inventory old selected highlight");

		auto oldCursorFrame = frame->addFrame("inventory old item cursor");
		oldCursorFrame->setSize(SDL_Rect{ 0, 0, inventorySlotSize + 16, inventorySlotSize + 16 });
		oldCursorFrame->setDisabled(true);
		color = SDL_MapRGBA(mainsurface->format, 255, 255, 255, oldSelectedCursorOpacity);
		oldCursorFrame->addImage(SDL_Rect{ 0, 0, 14, 14 },
			color, "images/ui/Inventory/SelectorGrey_TL.png", "inventory old cursor topleft");
		oldCursorFrame->addImage(SDL_Rect{ 0, 0, 14, 14 },
			color, "images/ui/Inventory/SelectorGrey_TR.png", "inventory old cursor topright");
		oldCursorFrame->addImage(SDL_Rect{ 0, 0, 14, 14 },
			color, "images/ui/Inventory/SelectorGrey_BL.png", "inventory old cursor bottomleft");
		oldCursorFrame->addImage(SDL_Rect{ 0, 0, 14, 14 },
			color, "images/ui/Inventory/SelectorGrey_BR.png", "inventory old cursor bottomright");

		auto cursorFrame = frame->addFrame("inventory selected item cursor");
		cursorFrame->setSize(SDL_Rect{ 0, 0, inventorySlotSize + 16, inventorySlotSize + 16 });
		cursorFrame->setDisabled(true);
		color = SDL_MapRGBA(mainsurface->format, 255, 255, 255, selectedCursorOpacity);
		cursorFrame->addImage(SDL_Rect{ 0, 0, 14, 14 },
			color, "images/ui/Inventory/Selector_TL.png", "inventory selected cursor topleft");
		cursorFrame->addImage(SDL_Rect{ 0, 0, 14, 14 },
			color, "images/ui/Inventory/Selector_TR.png", "inventory selected cursor topright");
		cursorFrame->addImage(SDL_Rect{ 0, 0, 14, 14 },
			color, "images/ui/Inventory/Selector_BL.png", "inventory selected cursor bottomleft");
		cursorFrame->addImage(SDL_Rect{ 0, 0, 14, 14 },
			color, "images/ui/Inventory/Selector_BR.png", "inventory selected cursor bottomright");
	}

	auto bgFrame = frame->findFrame("inventory base");
	auto flourishFrame = frame->addFrame("inventory base flourish");
	flourishFrame->setSize(SDL_Rect{ (bgFrame->getSize().w / 2) - (122 / 2), 202 - 22 + 6, 122, 22 });
	auto flourishImg = flourishFrame->addImage(SDL_Rect{ 0, 0, flourishFrame->getSize().w, flourishFrame->getSize().h },
		SDL_MapRGBA(mainsurface->format, 255, 255, 255, 255),
		"images/ui/Inventory/HUD_Inventory_Flourish_00.png", "inventory flourish img");

	{
		auto draggingInventoryItem = frame->addFrame("dragging inventory item");
		draggingInventoryItem->setSize(SDL_Rect{ 0, 0, inventorySlotSize, inventorySlotSize });
		draggingInventoryItem->setActualSize(SDL_Rect{ 0, 0, draggingInventoryItem->getSize().w, draggingInventoryItem->getSize().h });
		draggingInventoryItem->setDisabled(true);
		createPlayerInventorySlotFrameElements(draggingInventoryItem);
	}
}

void Player::Inventory_t::updateSelectedSlotAnimation(int destx, int desty, int width, int height, bool usingMouse)
{
	if ( frame )
	{
		if ( auto selectedSlotCursor = frame->findFrame("inventory selected item cursor") )
		{
			if ( usingMouse )
			{
				selectedSlotCursor->setSize(
					SDL_Rect{
					destx - cursor.cursorToSlotOffset,
					desty - cursor.cursorToSlotOffset,
					width + 2 * (cursor.cursorToSlotOffset + 1),
					height + 2 * (cursor.cursorToSlotOffset + 1)
				}
				);
				cursor.animateSetpointX = destx;
				cursor.animateSetpointY = desty;
				cursor.animateStartX = destx;
				cursor.animateStartY = desty;
			}
			else if ( cursor.animateSetpointX != destx || cursor.animateSetpointY != desty )
			{
				SDL_Rect size = selectedSlotCursor->getSize();
				cursor.animateStartX = size.x;
				cursor.animateStartY = size.y;
				size.w = width + 2 * (cursor.cursorToSlotOffset + 1);
				size.h = height + 2 * (cursor.cursorToSlotOffset + 1);
				selectedSlotCursor->setSize(size);
				cursor.animateSetpointX = destx;
				cursor.animateSetpointY = desty;
				cursor.animateX = 0.0;
				cursor.animateY = 0.0;
				cursor.lastUpdateTick = ticks;
			}
		}
	}
}

void Player::Hotbar_t::updateSelectedSlotAnimation(int destx, int desty, int width, int height, bool usingMouse)
{
	if ( hotbarFrame )
	{
		if ( auto selectedSlotCursor = hotbarFrame->findFrame("shootmode selected item cursor") )
		{
			if ( usingMouse )
			{
				selectedSlotCursor->setSize(
					SDL_Rect{
					destx - shootmodeCursor.cursorToSlotOffset,
					desty - shootmodeCursor.cursorToSlotOffset,
					width + 2 * (shootmodeCursor.cursorToSlotOffset + 1),
					height + 2 * (shootmodeCursor.cursorToSlotOffset + 1)
				}
				);
				shootmodeCursor.animateSetpointX = destx;
				shootmodeCursor.animateSetpointY = desty;
				shootmodeCursor.animateStartX = destx;
				shootmodeCursor.animateStartY = desty;
			}
			else if ( shootmodeCursor.animateSetpointX != destx || shootmodeCursor.animateSetpointY != desty )
			{
				SDL_Rect size = selectedSlotCursor->getSize();
				shootmodeCursor.animateStartX = size.x;
				shootmodeCursor.animateStartY = size.y;
				size.w = width + 2 * (shootmodeCursor.cursorToSlotOffset + 1);
				size.h = height + 2 * (shootmodeCursor.cursorToSlotOffset + 1);
				selectedSlotCursor->setSize(size);
				shootmodeCursor.animateSetpointX = destx;
				shootmodeCursor.animateSetpointY = desty;
				shootmodeCursor.animateX = 0.0;
				shootmodeCursor.animateY = 0.0;
				shootmodeCursor.lastUpdateTick = ticks;
			}
		}
	}
}

void Player::Inventory_t::updateSelectedItemAnimation()
{
	if ( frame )
	{
		if ( auto selectedSlotFrame = frame->findFrame("inventory selected item") )
		{
			selectedSlotFrame->setDisabled(true);
		}
		if ( auto selectedSlotCursor = frame->findFrame("inventory selected item cursor") )
		{
			selectedSlotCursor->setDisabled(true);
		}
	}
	if ( inputs.getUIInteraction(player.playernum)->selectedItem )
	{
		const real_t fpsScale = (144.f / std::max(1U, fpsLimit));
		real_t setpointDiffX = fpsScale * std::max(.05, (1.0 - selectedItemAnimate.animateX)) / (5);
		real_t setpointDiffY = fpsScale * std::max(.05, (1.0 - selectedItemAnimate.animateY)) / (5);
		selectedItemAnimate.animateX += setpointDiffX;
		selectedItemAnimate.animateY += setpointDiffY;
		selectedItemAnimate.animateX = std::min(1.0, selectedItemAnimate.animateX);
		selectedItemAnimate.animateY = std::min(1.0, selectedItemAnimate.animateY);
	}
	else
	{
		selectedItemAnimate.animateX = 0.0;
		selectedItemAnimate.animateY = 0.0;
	}
}

void Player::Inventory_t::updateInventoryItemTooltip()
{
	if ( !tooltipFrame || !frame )
	{
		return;
	}

	auto& tooltipDisplay = this->itemTooltipDisplay;

	if ( static_cast<int>(tooltipFrame->getOpacity()) != tooltipDisplay.opacitySetpoint )
	{
		const real_t fpsScale = (144.f / std::max(1U, fpsLimit));
		if ( tooltipDisplay.opacitySetpoint == 0 )
		{
			real_t setpointDiff = fpsScale * std::max(.05, (tooltipDisplay.opacityAnimate)) / (5);
			tooltipDisplay.opacityAnimate -= setpointDiff;
			tooltipDisplay.opacityAnimate = std::max(0.0, tooltipDisplay.opacityAnimate);
		}
		else
		{
			real_t setpointDiff = fpsScale * std::max(.05, (1.0 - tooltipDisplay.opacityAnimate)) / (1);
			tooltipDisplay.opacityAnimate += setpointDiff;
			tooltipDisplay.opacityAnimate = std::min(1.0, tooltipDisplay.opacityAnimate);
		}
		tooltipFrame->setOpacity(tooltipDisplay.opacityAnimate * 100);
	}

	tooltipDisplay.expandSetpoint = tooltipDisplay.expanded ? 100 : 0;
	if ( static_cast<int>(tooltipDisplay.expandCurrent * 100) != tooltipDisplay.expandSetpoint )
	{
		const real_t fpsScale = (144.f / std::max(1U, fpsLimit));
		if ( tooltipDisplay.expandSetpoint == 0 )
		{
			//real_t setpointDiff = fpsScale * std::max(.05, (tooltipDisplay.expandAnimate) / 50);
			//tooltipDisplay.expandAnimate -= setpointDiff;
			tooltipDisplay.expandAnimate -= 2 * fpsScale / 100.0;
			tooltipDisplay.expandAnimate = std::max(0.0, tooltipDisplay.expandAnimate);
		}
		else
		{
			//real_t setpointDiff = fpsScale * std::max(.05, (1.0 - tooltipDisplay.expandAnimate) / 50);
			//tooltipDisplay.expandAnimate += setpointDiff;
			tooltipDisplay.expandAnimate += 2 * fpsScale / 100.0;
			tooltipDisplay.expandAnimate = std::min(1.0, tooltipDisplay.expandAnimate);
		}
		double t = tooltipDisplay.expandAnimate;
		tooltipDisplay.expandCurrent = t * t * (3.0f - 2.0f * t); // bezier from 0 to width as t (0-1);
	}
}

void Player::Inventory_t::ItemTooltipDisplay_t::updateItem(const int player, Item* newItem)
{
	if ( newItem && player >= 0 && player < MAXPLAYERS && stats[player] )
	{
		uid = newItem->uid;
		type = newItem->type;
		status = newItem->status;
		beatitude = newItem->beatitude;
		count = newItem->count;
		appearance = newItem->appearance;
		identified = newItem->identified;

		if ( stats[player] )
		{
			playernum = player;
			playerLVL = stats[player]->LVL;
			playerEXP = stats[player]->EXP;
			playerSTR = statGetSTR(stats[player], players[player]->entity);
			playerDEX = statGetDEX(stats[player], players[player]->entity);
			playerCON = statGetCON(stats[player], players[player]->entity);
			playerINT = statGetINT(stats[player], players[player]->entity);
			playerPER = statGetPER(stats[player], players[player]->entity);
			playerCHR = statGetCHR(stats[player], players[player]->entity);
		}
	}
}

bool Player::Inventory_t::ItemTooltipDisplay_t::isItemSameAsCurrent(const int player, Item* newItem)
{
	if ( newItem && player >= 0 && player < MAXPLAYERS && stats[player] )
	{
		if ( newItem->uid == uid
			&& newItem->type == type
			&& newItem->status == status
			&& newItem->beatitude == beatitude
			&& newItem->count == count
			&& newItem->appearance == appearance
			&& newItem->identified == identified
			&& playernum == player
			&& playerLVL == stats[player]->LVL
			&& playerEXP == stats[player]->EXP
			&& playerSTR == statGetSTR(stats[player], players[player]->entity)
			&& playerDEX == statGetDEX(stats[player], players[player]->entity)
			&& playerCON == statGetCON(stats[player], players[player]->entity)
			&& playerINT == statGetINT(stats[player], players[player]->entity)
			&& playerPER == statGetPER(stats[player], players[player]->entity)
			&& playerCHR == statGetCHR(stats[player], players[player]->entity)
		)
		{
			return true;
		}
	}
	return false;
}

Player::Inventory_t::ItemTooltipDisplay_t::ItemTooltipDisplay_t()
{
	uid = 0;
	type = WOODEN_SHIELD;
	status = BROKEN;
	beatitude = 0;
	count = 0;
	appearance = 0;
	identified = false;

	playernum = -1;
	playerLVL = 0;
	playerEXP = 0;
	playerSTR = 0;
	playerDEX = 0;
	playerCON = 0;
	playerINT = 0;
	playerPER = 0;
	playerCHR = 0;
}

void Player::Inventory_t::updateCursor()
{
	if ( !frame )
	{
		return;
	}

	if ( auto oldSelectedSlotCursor = frame->findFrame("inventory old item cursor") )
	{
		if ( auto oldSelectedFrame = frame->findFrame("inventory old selected item") )
		{
			oldSelectedSlotCursor->setDisabled(oldSelectedFrame->isDisabled());

			if ( !oldSelectedSlotCursor->isDisabled() )
			{
				SDL_Rect cursorSize = oldSelectedSlotCursor->getSize();
				cursorSize.x = (oldSelectedFrame->getSize().x - 1) - cursor.cursorToSlotOffset;
				cursorSize.y = (oldSelectedFrame->getSize().y - 1) - cursor.cursorToSlotOffset;
				oldSelectedSlotCursor->setSize(cursorSize);

				int offset = 8;// ((ticks - cursor.lastUpdateTick) % 50 < 25) ? largeOffset : smallOffset;

				Uint8 r, g, b, a;
				if ( auto tl = oldSelectedSlotCursor->findImage("inventory old cursor topleft") )
				{
					tl->pos = SDL_Rect{ offset, offset, tl->pos.w, tl->pos.h };
					SDL_GetRGBA(tl->color, mainsurface->format, &r, &g, &b, &a);
					a = oldSelectedCursorOpacity;
					tl->color = SDL_MapRGBA(mainsurface->format, r, g, b, a);
				}
				if ( auto tr = oldSelectedSlotCursor->findImage("inventory old cursor topright") )
				{
					tr->pos = SDL_Rect{ -offset + cursorSize.w - tr->pos.w, offset, tr->pos.w, tr->pos.h };
					tr->color = SDL_MapRGBA(mainsurface->format, r, g, b, a);
				}
				if ( auto bl = oldSelectedSlotCursor->findImage("inventory old cursor bottomleft") )
				{
					bl->pos = SDL_Rect{ offset, -offset + cursorSize.h - bl->pos.h, bl->pos.w, bl->pos.h };
					bl->color = SDL_MapRGBA(mainsurface->format, r, g, b, a);
				}
				if ( auto br = oldSelectedSlotCursor->findImage("inventory old cursor bottomright") )
				{
					br->pos = SDL_Rect{ -offset + cursorSize.w - br->pos.w, -offset + cursorSize.h - br->pos.h, br->pos.w, br->pos.h };
					br->color = SDL_MapRGBA(mainsurface->format, r, g, b, a);
				}
			}
		}
	}

	if ( auto selectedSlotCursor = frame->findFrame("inventory selected item cursor") )
	{
		SDL_Rect cursorSize = selectedSlotCursor->getSize();

		const int smallOffset = 2;
		const int largeOffset = 4;

		int offset = ((ticks - cursor.lastUpdateTick) % 50 < 25) ? largeOffset : smallOffset;
		if ( inputs.getVirtualMouse(player.playernum)->draw_cursor )
		{
			if ( inputs.getUIInteraction(player.playernum)->selectedItem )
			{
				//offset = largeOffset;
			}
			else
			{
				offset = smallOffset;
			}
		}

		Uint8 r, g, b, a;
		if ( auto tl = selectedSlotCursor->findImage("inventory selected cursor topleft") )
		{
			tl->pos = SDL_Rect{ offset, offset, tl->pos.w, tl->pos.h };
			SDL_GetRGBA(tl->color, mainsurface->format, &r, &g, &b, &a);
			a = selectedCursorOpacity;
			tl->color = SDL_MapRGBA(mainsurface->format, r, g, b, a);
		}
		if ( auto tr = selectedSlotCursor->findImage("inventory selected cursor topright") )
		{
			tr->pos = SDL_Rect{ -offset + cursorSize.w - tr->pos.w, offset, tr->pos.w, tr->pos.h };
			tr->color = SDL_MapRGBA(mainsurface->format, r, g, b, a);
		}
		if ( auto bl = selectedSlotCursor->findImage("inventory selected cursor bottomleft") )
		{
			bl->pos = SDL_Rect{ offset, -offset + cursorSize.h - bl->pos.h, bl->pos.w, bl->pos.h };
			bl->color = SDL_MapRGBA(mainsurface->format, r, g, b, a);
		}
		if ( auto br = selectedSlotCursor->findImage("inventory selected cursor bottomright") )
		{
			br->pos = SDL_Rect{ -offset + cursorSize.w - br->pos.w, -offset + cursorSize.h - br->pos.h, br->pos.w, br->pos.h };
			br->color = SDL_MapRGBA(mainsurface->format, r, g, b, a);
		}

		SDL_Rect currentPos = selectedSlotCursor->getSize();
		const int offsetPosition = cursor.cursorToSlotOffset;
		if ( cursor.animateSetpointX - offsetPosition != currentPos.x
			|| cursor.animateSetpointY - offsetPosition != currentPos.y )
		{
			const real_t fpsScale = (50.f / std::max(1U, fpsLimit)); // ported from 50Hz
			real_t setpointDiffX = fpsScale * std::max(.1, (1.0 - cursor.animateX)) / (2.5);
			real_t setpointDiffY = fpsScale * std::max(.1, (1.0 - cursor.animateY)) / (2.5);
			cursor.animateX += setpointDiffX;
			cursor.animateY += setpointDiffY;
			cursor.animateX = std::min(1.0, cursor.animateX);
			cursor.animateY = std::min(1.0, cursor.animateY);

			int destX = cursor.animateSetpointX - cursor.animateStartX - offsetPosition;
			int destY = cursor.animateSetpointY - cursor.animateStartY - offsetPosition;

			currentPos.x = cursor.animateStartX + destX * cursor.animateX;
			currentPos.y = cursor.animateStartY + destY * cursor.animateY;
			selectedSlotCursor->setSize(currentPos);
			//messagePlayer(0, "%.2f | %.2f", inventory_t.selectedSlotAnimateX, setpointDiffX);
		}
	}
}

void Player::Hotbar_t::updateCursor()
{
	if ( !hotbarFrame )
	{
		return;
	}

	//if ( auto oldSelectedSlotCursor = hotbarFrame->findFrame("inventory old item cursor") )
	//{
	//	if ( auto oldSelectedFrame = hotbarFrame->findFrame("inventory old selected item") )
	//	{
	//		oldSelectedSlotCursor->setDisabled(oldSelectedFrame->isDisabled());

	//		if ( !oldSelectedSlotCursor->isDisabled() )
	//		{
	//			SDL_Rect cursorSize = oldSelectedSlotCursor->getSize();
	//			cursorSize.x = (oldSelectedFrame->getSize().x - 1) - shootmodeCursor.cursorToSlotOffset;
	//			cursorSize.y = (oldSelectedFrame->getSize().y - 1) - shootmodeCursor.cursorToSlotOffset;
	//			oldSelectedSlotCursor->setSize(cursorSize);

	//			int offset = 8;// ((ticks - shootmodeCursor.lastUpdateTick) % 50 < 25) ? largeOffset : smallOffset;

	//			Uint8 r, g, b, a;
	//			if ( auto tl = oldSelectedSlotCursor->findImage("inventory old cursor topleft") )
	//			{
	//				tl->pos = SDL_Rect{ offset, offset, tl->pos.w, tl->pos.h };
	//				SDL_GetRGBA(tl->color, mainsurface->format, &r, &g, &b, &a);
	//				a = oldSelectedCursorOpacity;
	//				tl->color = SDL_MapRGBA(mainsurface->format, r, g, b, a);
	//			}
	//			if ( auto tr = oldSelectedSlotCursor->findImage("inventory old cursor topright") )
	//			{
	//				tr->pos = SDL_Rect{ -offset + cursorSize.w - tr->pos.w, offset, tr->pos.w, tr->pos.h };
	//				tr->color = SDL_MapRGBA(mainsurface->format, r, g, b, a);
	//			}
	//			if ( auto bl = oldSelectedSlotCursor->findImage("inventory old cursor bottomleft") )
	//			{
	//				bl->pos = SDL_Rect{ offset, -offset + cursorSize.h - bl->pos.h, bl->pos.w, bl->pos.h };
	//				bl->color = SDL_MapRGBA(mainsurface->format, r, g, b, a);
	//			}
	//			if ( auto br = oldSelectedSlotCursor->findImage("inventory old cursor bottomright") )
	//			{
	//				br->pos = SDL_Rect{ -offset + cursorSize.w - br->pos.w, -offset + cursorSize.h - br->pos.h, br->pos.w, br->pos.h };
	//				br->color = SDL_MapRGBA(mainsurface->format, r, g, b, a);
	//			}
	//		}
	//	}
	//}


	if ( auto selectedSlotCursor = hotbarFrame->findFrame("shootmode selected item cursor") )
	{
		SDL_Rect cursorSize = selectedSlotCursor->getSize();

		const int smallOffset = 2;
		const int largeOffset = 4;

		int offset = ((ticks - shootmodeCursor.lastUpdateTick) % 50 < 25) ? largeOffset : smallOffset;
		if ( inputs.getVirtualMouse(player.playernum)->draw_cursor )
		{
			if ( inputs.getUIInteraction(player.playernum)->selectedItem )
			{
				//offset = largeOffset;
			}
			else
			{
				offset = smallOffset;
			}
		}

		Uint8 r, g, b, a;
		if ( auto tl = selectedSlotCursor->findImage("shootmode selected cursor topleft") )
		{
			tl->pos = SDL_Rect{ offset, offset, tl->pos.w, tl->pos.h };
			SDL_GetRGBA(tl->color, mainsurface->format, &r, &g, &b, &a);
			a = selectedCursorOpacity;
			tl->color = SDL_MapRGBA(mainsurface->format, r, g, b, a);
		}
		if ( auto tr = selectedSlotCursor->findImage("shootmode selected cursor topright") )
		{
			tr->pos = SDL_Rect{ -offset + cursorSize.w - tr->pos.w, offset, tr->pos.w, tr->pos.h };
			tr->color = SDL_MapRGBA(mainsurface->format, r, g, b, a);
		}
		if ( auto bl = selectedSlotCursor->findImage("shootmode selected cursor bottomleft") )
		{
			bl->pos = SDL_Rect{ offset, -offset + cursorSize.h - bl->pos.h, bl->pos.w, bl->pos.h };
			bl->color = SDL_MapRGBA(mainsurface->format, r, g, b, a);
		}
		if ( auto br = selectedSlotCursor->findImage("shootmode selected cursor bottomright") )
		{
			br->pos = SDL_Rect{ -offset + cursorSize.w - br->pos.w, -offset + cursorSize.h - br->pos.h, br->pos.w, br->pos.h };
			br->color = SDL_MapRGBA(mainsurface->format, r, g, b, a);
		}

		SDL_Rect currentPos = selectedSlotCursor->getSize();
		const int offsetPosition = shootmodeCursor.cursorToSlotOffset;
		if ( shootmodeCursor.animateSetpointX - offsetPosition != currentPos.x
			|| shootmodeCursor.animateSetpointY - offsetPosition != currentPos.y )
		{
			auto& cursor = shootmodeCursor;
			const real_t fpsScale = (50.f / std::max(1U, fpsLimit)); // ported from 50Hz
			real_t setpointDiffX = fpsScale * std::max(.1, (1.0 - cursor.animateX)) / (2.5);
			real_t setpointDiffY = fpsScale * std::max(.1, (1.0 - cursor.animateY)) / (2.5);
			cursor.animateX += setpointDiffX;
			cursor.animateY += setpointDiffY;
			cursor.animateX = std::min(1.0, cursor.animateX);
			cursor.animateY = std::min(1.0, cursor.animateY);

			int destX = cursor.animateSetpointX - cursor.animateStartX - offsetPosition;
			int destY = cursor.animateSetpointY - cursor.animateStartY - offsetPosition;

			currentPos.x = cursor.animateStartX + destX * cursor.animateX;
			currentPos.y = cursor.animateStartY + destY * cursor.animateY;
			selectedSlotCursor->setSize(currentPos);
			//messagePlayer(0, "%.2f | %.2f", inventory_t.selectedSlotAnimateX, setpointDiffX);
		}
	}
}

void Player::Inventory_t::processInventory()
{
	if ( !frame )
	{
		createPlayerInventory(player.playernum);
	}

	frame->setSize(SDL_Rect{ players[player.playernum]->camera_x1(),
		players[player.playernum]->camera_y1(),
		players[player.playernum]->camera_width(),
		players[player.playernum]->camera_height()});
	frame->setActualSize(SDL_Rect{0, 0, frame->getSize().w, frame->getSize().h});

	bool tooltipWasDisabled = tooltipFrame->isDisabled();

	updateInventory();

	if ( tooltipWasDisabled && !tooltipFrame->isDisabled() )
	{
		tooltipFrame->setOpacity(0.0);
	}
}

void Player::HUD_t::resetBars()
{
	if ( xpFrame )
	{
		xpBar.animateSetpoint = std::min(100, stats[player.playernum]->EXP) * 10;
		xpBar.animateValue = xpBar.animateSetpoint;
		xpBar.animatePreviousSetpoint = xpBar.animateSetpoint;
		xpBar.animateState = ANIMATE_NONE;
		xpBar.xpLevelups = 0;
	}
	if ( hpFrame )
	{
		HPBar.animateSetpoint = stats[player.playernum]->HP;
		HPBar.animateValue = HPBar.animateSetpoint;
		HPBar.animateValue2 = HPBar.animateSetpoint;
		HPBar.animatePreviousSetpoint = HPBar.animateSetpoint;
		HPBar.animateState = ANIMATE_NONE;
	}
	if ( mpFrame )
	{
		MPBar.animateSetpoint = stats[player.playernum]->MP;
		MPBar.animateValue = MPBar.animateSetpoint;
		MPBar.animateValue2 = MPBar.animateSetpoint;
		MPBar.animatePreviousSetpoint = MPBar.animateSetpoint;
		MPBar.animateState = ANIMATE_NONE;
	}
}

void Player::HUD_t::updateXPBar()
{
	if ( !xpFrame )
	{
		return;
	}

	xpBar.animateSetpoint = std::min(100, stats[player.playernum]->EXP);
	xpBar.maxValue = 1000.0;

	if ( xpBar.animateState == ANIMATE_LEVELUP_RISING )
	{
		xpBar.animateTicks = ticks;

		real_t fpsScale = (144.f / std::max(1U, fpsLimit));
		xpBar.animateValue += fpsScale * (10); // constant speed
		xpBar.animateValue = std::min(xpBar.maxValue, xpBar.animateValue);
		if ( xpBar.animateValue == xpBar.maxValue )
		{
			xpBar.animateState = ANIMATE_LEVELUP_FALLING;
		}
	}
	else if ( xpBar.animateState == ANIMATE_LEVELUP_FALLING )
	{
		if ( ticks - xpBar.animateTicks > TICKS_PER_SECOND * 2 )
		{
			int decrement = 40;
			double scaledDecrement = (decrement * (144.f / std::max(1U, fpsLimit)));
			xpBar.animateValue -= scaledDecrement;
			if ( xpBar.animateValue <= 0 )
			{
				xpBar.animateValue = 0.0;
				xpBar.xpLevelups = 0; // disable looping maybe one day use it
				if ( xpBar.xpLevelups > 1 )
				{
					--xpBar.xpLevelups;
					xpBar.animateState = ANIMATE_LEVELUP_RISING;
				}
				else
				{
					xpBar.xpLevelups = 0;
					xpBar.animateState = ANIMATE_NONE;
				}
			}
		}
	}
	else
	{
		int increment = 3;
		double scaledIncrement = (increment * (144.f / std::max(1U, fpsLimit)));
		if ( xpBar.animateValue < xpBar.animateSetpoint * 10 )
		{
			//real_t diff = std::max(.1, (xpBar.animateSetpoint * 10 - xpBar.animateValue) / 200.0); // 0.1-5 value
			//if ( xpBar.animateSetpoint * 10 >= xpBar.maxValue )
			//{
			//	diff = 5;
			//}
			//scaledIncrement *= 0.2 * pow(diff, 2) + .5;
			//xpBar.animateValue = std::min(xpBar.animateSetpoint * 10.0, xpBar.animateValue + scaledIncrement);

			real_t setpointDiff = std::max(50.0, xpBar.animateSetpoint * 10.0 - xpBar.animateValue);
			real_t fpsScale = (144.f / std::max(1U, fpsLimit));
			xpBar.animateValue += fpsScale * (setpointDiff / 50.0); // reach it in x intervals, scaled to FPS
			xpBar.animateValue = std::min(static_cast<real_t>(xpBar.animateSetpoint * 10.0), xpBar.animateValue);
			//messagePlayer(0, "%.2f | %.2f", diff, scaledIncrement);
		}
		//else if ( xpBar.animateValue > xpBar.animateSetpoint * 10 )
		//{
		//	real_t fpsScale = (144.f / std::max(1U, fpsLimit));
		//	xpBar.animateValue += fpsScale * (10); // constant speed
		//	xpBar.animateValue = std::min(xpBar.maxValue, xpBar.animateValue);
		//}
		//else
		//{
		//	xpBar.animateTicks = ticks;
		//}

		xpBar.animateTicks = ticks;

		//if ( xpBar.animateValue == xpBar.maxValue )
		//{
		//	xpBar.animateState = ANIMATE_LEVELUP;
		//}
	}

	char playerXPText[16];
	snprintf(playerXPText, sizeof(playerXPText), "%.f", xpBar.animateValue / 10);
	
	auto xpText = xpFrame->findField("xp text current");
	xpText->setText(playerXPText);

	auto xpBg = xpFrame->findImage("xp img base");
	auto xpProgress = xpFrame->findImage("xp img progress");
	auto xpProgressEndCap = xpFrame->findImage("xp img progress endcap");

	real_t percent = xpBar.animateValue / 1000.0;
	xpProgress->pos.w = std::max(1, static_cast<int>((xpBg->pos.w - xpProgressEndCap->pos.w) * percent));
	xpProgressEndCap->pos.x = xpProgress->pos.x + xpProgress->pos.w;
}

void Player::HUD_t::updateHPBar()
{
	if ( !hpFrame )
	{
		return;
	}

	auto hpForegroundFrame = hpFrame->findFrame("hp foreground frame");
	auto hpBg = hpFrame->findImage("hp img base");
	auto hpEndcap = hpForegroundFrame->findImage("hp img endcap");
	auto hpProgressBot = hpForegroundFrame->findImage("hp img progress bot");
	auto hpProgress = hpForegroundFrame->findImage("hp img progress");
	auto hpProgressEndCap = hpForegroundFrame->findImage("hp img progress endcap");
	auto hpFadeFrame = hpFrame->findFrame("hp fade frame");
	auto hpFadedBase = hpFadeFrame->findImage("hp img fade bot");
	auto hpFaded = hpFadeFrame->findImage("hp img fade");
	auto hpFadedEndCap = hpFadeFrame->findImage("hp img fade endcap");

	real_t progressWidth = hpFrame->getSize().w - 74;
	int backgroundWidth = hpFrame->getSize().w - 54;

	// handle bar size changing
	{
		real_t multiplier = 1.0;
		const Sint32 maxHPWidth = 160;
		if ( stats[player.playernum]->MAXHP < maxHPWidth )
		{
			// start at 30%, increase 2.5% every 5 HP past 20 MAXHP
			multiplier = .3 + (.025 * ((std::max(0, stats[player.playernum]->MAXHP - 20) / 5)));
		}

		int diff = static_cast<int>(std::max(0.0, progressWidth - progressWidth * multiplier)); // how many pixels the progress bar shrinks
		progressWidth *= multiplier; // scale the progress bars
		hpBg->pos.w = backgroundWidth - diff; // move the background bar by x pixels as above
		hpEndcap->pos.x = hpFrame->getSize().w - hpEndcap->pos.w - diff; // move the background endcap by x pixels as above
	}

	HPBar.animatePreviousSetpoint = HPBar.animateSetpoint;
	real_t& hpForegroundValue = HPBar.animateValue;
	real_t& hpFadedValue = HPBar.animateValue2;

	HPBar.animateSetpoint = stats[player.playernum]->HP;
	if ( HPBar.animateSetpoint < HPBar.animatePreviousSetpoint ) // insta-change as losing health
	{
		hpForegroundValue = HPBar.animateSetpoint;
		HPBar.animateTicks = ticks;
	}

	if ( HPBar.maxValue > stats[player.playernum]->MAXHP )
	{
		hpFadedValue = HPBar.animateSetpoint; // resetting game etc, stop fade animation sticking out of frame
	}

	HPBar.maxValue = stats[player.playernum]->MAXHP;

	if ( hpForegroundValue < HPBar.animateSetpoint ) // gaining HP, animate
	{
		real_t setpointDiff = std::max(0.0, HPBar.animateSetpoint - hpForegroundValue);
		real_t fpsScale = (144.f / std::max(1U, fpsLimit));
		hpForegroundValue += fpsScale * (setpointDiff / 20.0); // reach it in 20 intervals, scaled to FPS
		hpForegroundValue = std::min(static_cast<real_t>(HPBar.animateSetpoint), hpForegroundValue);

		if ( abs(HPBar.animateSetpoint) - abs(hpForegroundValue) <= 1.0 )
		{
			hpForegroundValue = HPBar.animateSetpoint;
		}

		/*	int increment = 3;
			double scaledIncrement = (increment * (144.f / std::max(1U, fpsLimit)));*/
		//real_t diff = std::max(.1, (HPBar.animateSetpoint * 10 - hpForegroundValue) / (maxValue / 5)); // 0.1-5 value
		//if ( HPBar.animateSetpoint * 10 >= maxValue )
		//{
		//	diff = 5;
		//}
		//scaledIncrement *= 0.2 * pow(diff, 2) + .5;

		//hpForegroundValue = std::min(HPBar.animateSetpoint * 10.0, hpForegroundValue + scaledIncrement);
		//messagePlayer(0, "%.2f | %.2f", hpForegroundValue);
	}
	else if ( hpForegroundValue > HPBar.animateSetpoint ) // losing HP, snap to value
	{
		hpForegroundValue = HPBar.animateSetpoint;
	}

	if ( hpFadedValue < HPBar.animateSetpoint )
	{
		hpFadedValue = hpForegroundValue;
		HPBar.animateTicks = ticks;
	}
	else if ( hpFadedValue > HPBar.animateSetpoint )
	{
		if ( ticks - HPBar.animateTicks > 30 /*|| stats[player.playernum]->HP <= 0*/ ) // fall after x ticks
		{
			real_t setpointDiff = std::max(0.01, hpFadedValue - HPBar.animateSetpoint);
			real_t fpsScale = (144.f / std::max(1U, fpsLimit));
			hpFadedValue -= fpsScale * (setpointDiff / 20.0); // reach it in 20 intervals, scaled to FPS
			hpFadedValue = std::max(static_cast<real_t>(HPBar.animateSetpoint), hpFadedValue);
		}
	}
	else
	{
		HPBar.animateTicks = ticks;
	}

	char playerHPText[16];
	snprintf(playerHPText, sizeof(playerHPText), "%d", stats[player.playernum]->HP);

	auto hpText = hpForegroundFrame->findField("hp text");
	hpText->setText(playerHPText);

	real_t foregroundPercent = hpForegroundValue / HPBar.maxValue;
	hpProgress->pos.w = std::max(1, static_cast<int>((progressWidth) * foregroundPercent));
	hpProgressEndCap->pos.x = hpProgress->pos.x + hpProgress->pos.w;

	real_t fadePercent = hpFadedValue / HPBar.maxValue;
	hpFaded->pos.w = std::max(1, static_cast<int>((progressWidth) * fadePercent));
	hpFadedEndCap->pos.x = hpFaded->pos.x + hpFaded->pos.w;
	if ( hpFaded->pos.w == 1 && stats[player.playernum]->HP <= 0 )
	{
		hpFaded->disabled = true;
		real_t opacity = hpFadeFrame->getOpacity();
		real_t opacityChange = .5 * (144.f / std::max(1U, fpsLimit)); // change by .05% independant of fps
		hpFadeFrame->setOpacity(std::max(0.0, opacity - opacityChange));

		// make this element fade out to the left, starting 54px then finally at 40px. @ 40px it's out of shot (6 width + 8 endcap width)
		hpFadedBase->pos.x = 54 - (14 * (1.0 - opacity / 100.0)); 
		hpFadedEndCap->pos.x = hpFadedBase->pos.x + hpFadedBase->pos.w;
	}
	else
	{
		hpFaded->disabled = false;
		hpFadeFrame->setOpacity(100.0);

		// reset to 54px left as we altered in above if statement
		hpFadedBase->pos.x = 54;
	}

	if ( stats[player.playernum]->HP <= 0 ) 
	{
		// hide all the progress elements when dead, as endcap/base don't shrink
		// hpProgress width 0px defaults to original size, so hide that too
		hpProgress->disabled = true;
		hpProgressEndCap->disabled = true;
		hpProgressBot->disabled = true;
	}
	else
	{
		hpProgress->disabled = false;
		hpProgressEndCap->disabled = false;
		hpProgressBot->disabled = false;
	}
}

void Player::HUD_t::updateMPBar()
{
	if ( !mpFrame )
	{
		return;
	}

	auto mpForegroundFrame = mpFrame->findFrame("mp foreground frame");
	auto mpBg = mpFrame->findImage("mp img base");
	auto mpEndcap = mpForegroundFrame->findImage("mp img endcap");
	auto mpProgressBot = mpForegroundFrame->findImage("mp img progress bot");
	auto mpProgress = mpForegroundFrame->findImage("mp img progress");
	auto mpProgressEndCap = mpForegroundFrame->findImage("mp img progress endcap");
	auto mpFadeFrame = mpFrame->findFrame("mp fade frame");
	auto mpFadedBase = mpFadeFrame->findImage("mp img fade bot");
	auto mpFaded = mpFadeFrame->findImage("mp img fade");
	auto mpFadedEndCap = mpFadeFrame->findImage("mp img fade endcap");

	real_t progressWidth = mpFrame->getSize().w - 74;
	int backgroundWidth = mpFrame->getSize().w - 54;

	// handle bar size changing
	{
		real_t multiplier = 1.0;
		const Sint32 maxMPWidth = 160;
		if ( stats[player.playernum]->MAXMP < maxMPWidth )
		{
			// start at 30%, increase 2.5% every 5 MP past 20 MAXMP
			multiplier = .3 + (.025 * ((std::max(0, stats[player.playernum]->MAXMP - 20) / 5)));
		}

		int diff = static_cast<int>(std::max(0.0, progressWidth - progressWidth * multiplier)); // how many pixels the progress bar shrinks
		progressWidth *= multiplier; // scale the progress bars
		mpBg->pos.w = backgroundWidth - diff; // move the background bar by x pixels as above
		mpEndcap->pos.x = mpFrame->getSize().w - mpEndcap->pos.w - diff; // move the background endcap by x pixels as above
	}

	MPBar.animatePreviousSetpoint = MPBar.animateSetpoint;
	real_t& mpForegroundValue = MPBar.animateValue;
	real_t& mpFadedValue = MPBar.animateValue2;

	MPBar.animateSetpoint = stats[player.playernum]->MP;
	if ( MPBar.animateSetpoint < MPBar.animatePreviousSetpoint ) // insta-change as losing health
	{
		mpForegroundValue = MPBar.animateSetpoint;
		MPBar.animateTicks = ticks;
	}

	if ( MPBar.maxValue > stats[player.playernum]->MAXMP )
	{
		mpFadedValue = MPBar.animateSetpoint; // resetting game etc, stop fade animation sticking out of frame
	}

	MPBar.maxValue = stats[player.playernum]->MAXMP;
	if ( mpForegroundValue < MPBar.animateSetpoint ) // gaining MP, animate
	{
		real_t setpointDiff = std::max(.1, MPBar.animateSetpoint - mpForegroundValue);
		real_t fpsScale = (144.f / std::max(1U, fpsLimit));
		mpForegroundValue += fpsScale * (setpointDiff / 20.0); // reach it in 20 intervals, scaled to FPS
		mpForegroundValue = std::min(static_cast<real_t>(MPBar.animateSetpoint), mpForegroundValue);

		/*if ( abs(MPBar.animateSetpoint) - abs(mpForegroundValue) <= 1.0 )
		{
			mpForegroundValue = MPBar.animateSetpoint;
		}*/
	}
	else if ( mpForegroundValue > MPBar.animateSetpoint ) // losing MP, snap to value
	{
		mpForegroundValue = MPBar.animateSetpoint;
	}

	if ( mpFadedValue < MPBar.animateSetpoint )
	{
		mpFadedValue = mpForegroundValue;
		MPBar.animateTicks = ticks;
	}
	else if ( mpFadedValue > MPBar.animateSetpoint )
	{
		if ( ticks - MPBar.animateTicks > 30 /*|| stats[player.playernum]->MP <= 0*/ ) // fall after x ticks
		{
			real_t setpointDiff = std::max(0.1, mpFadedValue - MPBar.animateSetpoint);
			real_t fpsScale = (144.f / std::max(1U, fpsLimit));
			mpFadedValue -= fpsScale * (setpointDiff / 20.0); // reach it in 20 intervals, scaled to FPS
			mpFadedValue = std::max(static_cast<real_t>(MPBar.animateSetpoint), mpFadedValue);
		}
	}
	else
	{
		MPBar.animateTicks = ticks;
	}

	char playerMPText[16];
	snprintf(playerMPText, sizeof(playerMPText), "%d", stats[player.playernum]->MP);

	auto mpText = mpForegroundFrame->findField("mp text");
	mpText->setText(playerMPText);

	real_t foregroundPercent = mpForegroundValue / MPBar.maxValue;
	mpProgress->pos.w = std::max(1, static_cast<int>((progressWidth)* foregroundPercent));
	mpProgressEndCap->pos.x = mpProgress->pos.x + mpProgress->pos.w;

	real_t fadePercent = mpFadedValue / MPBar.maxValue;
	mpFaded->pos.w = std::max(1, static_cast<int>((progressWidth)* fadePercent));
	mpFadedEndCap->pos.x = mpFaded->pos.x + mpFaded->pos.w;
	if ( mpFaded->pos.w == 1 && stats[player.playernum]->MP <= 0 )
	{
		mpFaded->disabled = true;
		real_t opacity = mpFadeFrame->getOpacity();
		real_t opacityChange = .5 * (144.f / std::max(1U, fpsLimit)); // change by .05% independant of fps
		mpFadeFrame->setOpacity(std::max(0.0, opacity - opacityChange));

		// make this element fade out to the left, starting 54px then finally at 40px. @ 40px it's out of shot (6 width + 8 endcap width)
		mpFadedBase->pos.x = 54 - (14 * (1.0 - opacity / 100.0));
		mpFadedEndCap->pos.x = mpFadedBase->pos.x + mpFadedBase->pos.w;
	}
	else
	{
		mpFaded->disabled = false;
		mpFadeFrame->setOpacity(100.0);

		// reset to 54px left as we altered in above if statement
		mpFadedBase->pos.x = 54;
	}

	if ( stats[player.playernum]->MP <= 0 )
	{
		// hide all the progress elements when dead, as endcap/base don't shrink
		// mpProgress width 0px defaults to original size, so hide that too
		mpProgress->disabled = true;
		mpProgressEndCap->disabled = true;
		mpProgressBot->disabled = true;
	}
	else
	{
		mpProgress->disabled = false;
		mpProgressEndCap->disabled = false;
		mpProgressBot->disabled = false;
	}
}

void Player::Hotbar_t::updateHotbar()
{
	if ( !hotbarFrame )
	{
		return;
	}

	const int hotbarStartY1 = Frame::virtualScreenY - 106; // higher row (center group)
	const int hotbarStartY2 = Frame::virtualScreenY - 96; // lower row (left/right)
	const int hotbarCentreX = Frame::virtualScreenX / 2;
	const int hotbarCentreXLeft = hotbarCentreX - 148;
	const int hotbarCentreXRight = hotbarCentreX + 148;

	if ( !player.shootmode )
	{
		if ( Input::inputs[player.shootmode].binaryToggle("HotbarFacebarCancel") )
		{
			Input::inputs[player.shootmode].consumeBinaryToggle("HotbarFacebarCancel");
		}
		if ( Input::inputs[player.shootmode].binaryToggle("HotbarFacebarLeft") )
		{
			Input::inputs[player.shootmode].consumeBinaryToggle("HotbarFacebarLeft");
			Input::inputs[player.shootmode].consumeBinaryReleaseToggle("HotbarFacebarLeft");
		}
		if ( Input::inputs[player.shootmode].binaryToggle("HotbarFacebarUp") )
		{
			Input::inputs[player.shootmode].consumeBinaryToggle("HotbarFacebarUp");
			Input::inputs[player.shootmode].consumeBinaryReleaseToggle("HotbarFacebarUp");
		}
		if ( Input::inputs[player.shootmode].binaryToggle("HotbarFacebarRight") )
		{
			Input::inputs[player.shootmode].consumeBinaryToggle("HotbarFacebarRight");
			Input::inputs[player.shootmode].consumeBinaryReleaseToggle("HotbarFacebarRight");
		}
		faceMenuButtonHeld = FaceMenuGroup::GROUP_NONE;
	}

	bool faceMenuSnapCursorInstantly = false;
	if ( faceMenuButtonHeld != FaceMenuGroup::GROUP_NONE )
	{
		if ( selectedSlotAnimateCurrentValue == 0.0 )
		{
			faceMenuSnapCursorInstantly = true;
		}
		real_t fpsScale = (144.f / std::max(1U, fpsLimit));
		real_t setpointDiff = std::max(0.1, 1.0 - selectedSlotAnimateCurrentValue);
		selectedSlotAnimateCurrentValue += fpsScale * (setpointDiff / 10.0);
		selectedSlotAnimateCurrentValue = std::min(1.0, selectedSlotAnimateCurrentValue);
	}
	else
	{
		selectedSlotAnimateCurrentValue = 0.0;
	}

	auto highlightSlot = hotbarFrame->findFrame("hotbar highlight");
	auto highlightSlotImg = highlightSlot->findImage("highlight img");
	highlightSlotImg->disabled = true;

	auto shootmodeSelectedSlotCursor = hotbarFrame->findFrame("shootmode selected item cursor");
	if ( shootmodeSelectedSlotCursor )
	{
		shootmodeSelectedSlotCursor->setDisabled(true);
	}

	// position the slots
	for ( int num = 0; num < NUM_HOTBAR_SLOTS; ++num )
	{
		char slotname[32];
		snprintf(slotname, sizeof(slotname), "hotbar slot %d", num);
		auto slot = hotbarFrame->findFrame(slotname);
		assert(slot);

		if ( auto img = slot->findImage("slot img") ) // apply any opacity from config
		{
			Uint8 r, g, b, a;
			SDL_GetRGBA(img->color, mainsurface->format, &r, &g, &b, &a);
			a = hotbarSlotOpacity;
			img->color = SDL_MapRGBA(mainsurface->format, r, g, b, a);
		}
		if ( highlightSlotImg )
		{
			Uint8 r, g, b, a;
			SDL_GetRGBA(highlightSlotImg->color, mainsurface->format, &r, &g, &b, &a);
			a = hotbarSelectedSlotOpacity;
			highlightSlotImg->color = SDL_MapRGBA(mainsurface->format, r, g, b, a);
		}

		char glyphname[32];
		snprintf(glyphname, sizeof(glyphname), "hotbar glyph %d", num);
		auto glyph = hotbarFrame->findImage(glyphname);
		assert(glyph);
		glyph->disabled = true;

		if ( useHotbarFaceMenu && num == 9 )
		{
			slot->setDisabled(true);
		}
		else
		{
			slot->setDisabled(false);
		}

		SDL_Rect pos = slot->getSize();
		pos.x = hotbarCentreX;
		pos.y = hotbarStartY2;

		int slotYMovement = pos.h / 4;

		auto slotItem = slot->findFrame("slot item");
		slotItem->setDisabled(true);

		if ( useHotbarFaceMenu )
		{
			GameController* controller = inputs.getController(player.playernum);
			if ( controller )
			{
				glyph->disabled = slot->isDisabled();
			}

			slot->findField("slot num text")->setDisabled(true); // disable the hotkey prompts per slot
			switch ( num )
			{
				// left group
				case 0:
					pos.x = hotbarCentreXLeft - pos.w / 2 - pos.w + 2;
					if ( faceMenuButtonHeld == FaceMenuGroup::GROUP_LEFT )
					{
						pos.y -= slotYMovement * selectedSlotAnimateCurrentValue;
					}
					else
					{
						glyph->disabled = true;
					}
					glyph->path = Input::inputs[player.playernum].getGlyphPathForInput("HotbarFacebarModifierLeft");
					break;
				case 1:
					pos.x = hotbarCentreXLeft - pos.w / 2;
					pos.y -= slotYMovement;
					glyph->path = Input::inputs[player.playernum].getGlyphPathForInput("HotbarFacebarLeft");
					break;
				case 2:
					pos.x = hotbarCentreXLeft + (pos.w / 2 - 2);
					if ( faceMenuButtonHeld == FaceMenuGroup::GROUP_LEFT )
					{
						pos.y -= slotYMovement * selectedSlotAnimateCurrentValue;
					}
					else
					{
						glyph->disabled = true;
					}
					glyph->path = Input::inputs[player.playernum].getGlyphPathForInput("HotbarFacebarModifierRight");
					break;
				// middle group
				case 3:
					pos.y = hotbarStartY1;
					pos.x = hotbarCentreX - pos.w / 2 - pos.w + 2;
					if ( faceMenuButtonHeld == FaceMenuGroup::GROUP_MIDDLE )
					{
						pos.y -= slotYMovement * selectedSlotAnimateCurrentValue;
					}
					else
					{
						glyph->disabled = true;
					}
					glyph->path = Input::inputs[player.playernum].getGlyphPathForInput("HotbarFacebarModifierLeft");
					break;
				case 4:
					pos.y = hotbarStartY1;
					pos.y -= slotYMovement;
					pos.x = hotbarCentreX - pos.w / 2;
					glyph->path = Input::inputs[player.playernum].getGlyphPathForInput("HotbarFacebarUp");
					break;
				case 5:
					pos.y = hotbarStartY1;
					pos.x = hotbarCentreX + (pos.w / 2 - 2);
					if ( faceMenuButtonHeld == FaceMenuGroup::GROUP_MIDDLE )
					{
						pos.y -= slotYMovement * selectedSlotAnimateCurrentValue;
					}
					else
					{
						glyph->disabled = true;
					}
					glyph->path = Input::inputs[player.playernum].getGlyphPathForInput("HotbarFacebarModifierRight");
					break;
				// right group
				case 6:
					pos.x = hotbarCentreXRight - pos.w / 2 - pos.w + 2;
					if ( faceMenuButtonHeld == FaceMenuGroup::GROUP_RIGHT )
					{
						pos.y -= slotYMovement * selectedSlotAnimateCurrentValue;
					}
					else
					{
						glyph->disabled = true;
					}
					glyph->path = Input::inputs[player.playernum].getGlyphPathForInput("HotbarFacebarModifierLeft");
					break;
				case 7:
					pos.x = hotbarCentreXRight - pos.w / 2;
					pos.y -= slotYMovement;
					glyph->path = Input::inputs[player.playernum].getGlyphPathForInput("HotbarFacebarRight");
					break;
				case 8:
					pos.x = hotbarCentreXRight + (pos.w / 2 - 2);
					if ( faceMenuButtonHeld == FaceMenuGroup::GROUP_RIGHT )
					{
						pos.y -= slotYMovement * selectedSlotAnimateCurrentValue;
					}
					else
					{
						glyph->disabled = true;
					}
					glyph->path = Input::inputs[player.playernum].getGlyphPathForInput("HotbarFacebarModifierRight");
					break;
				default:
					break;
			}
		}
		else
		{
			slot->findField("slot num text")->setDisabled(false); // enable the hotkey prompts per slot
			const unsigned int midpoint = NUM_HOTBAR_SLOTS / 2;
			if ( num < midpoint )
			{
				pos.x -= (pos.w) * (midpoint - num);
			}
			else
			{
				pos.x += (pos.w) * (num - midpoint);
			}
		}

		slot->setSize(pos);

		auto glyphImage = Image::get(glyph->path.c_str());
		if ( glyphImage )
		{
			glyph->pos.w = glyphImage->getWidth();
			glyph->pos.h = glyphImage->getHeight();
			glyph->pos.x = pos.x + pos.w / 2 - glyph->pos.w / 2;
			glyph->pos.y = pos.y - glyph->pos.h;
		}

		if ( current_hotbar == num )
		{
			bool showHighlightedSlot = true;
			if ( !player.hotbar.hotbarHasFocus && player.inventoryUI.frame && !player.inventoryUI.frame->isDisabled() )
			{
				// if inventory visible, don't show selection if navigating within inventory
				showHighlightedSlot = false;
			}
			else if ( player.hotbar.useHotbarFaceMenu && inputs.getVirtualMouse(player.playernum)->lastMovementFromController
				&& player.hotbar.faceMenuButtonHeld == Player::Hotbar_t::FaceMenuGroup::GROUP_NONE
				&& player.inventoryUI.frame && player.inventoryUI.frame->isDisabled() )
			{
				// if inventory invisible, don't show selection if face button not held
				showHighlightedSlot = false;
			}

			auto highlightSlotItem = highlightSlot->findFrame("slot item");
			highlightSlotItem->setDisabled(true);

			if ( showHighlightedSlot )
			{
				auto slotNumText = slot->findField("slot num text");
				auto highlightNumText = highlightSlot->findField("slot num text");

				highlightNumText->setText(slotNumText->getText());
				highlightNumText->setDisabled(slotNumText->isDisabled());

				highlightSlot->setSize(pos); // this follows the slots around
				highlightSlotImg->disabled = false;
				updateSlotFrameFromItem(highlightSlotItem, uidToItem(hotbar[num].item));

				if ( player.inventoryUI.frame )
				{
					bool showCursor = true;
					if ( !player.shootmode )
					{
						if ( inputs.getUIInteraction(player.playernum)->selectedItem 
							&& !highlightSlot->capturesMouseInRealtimeCoords() )
						{
							showCursor = false;
						}
						else if ( !inputs.getUIInteraction(player.playernum)->selectedItem
							&& !inputs.getVirtualMouse(player.playernum)->lastMovementFromController
							&& !highlightSlot->capturesMouse() )
						{
							showCursor = false;
						}
					}
					else if ( player.shootmode )
					{
						showCursor = true;
					}

					if ( showCursor )
					{
						if ( !player.shootmode )
						{
							if ( auto selectedSlotCursor = player.inventoryUI.frame->findFrame("inventory selected item cursor") )
							{
								selectedSlotCursor->setDisabled(false);
								player.inventoryUI.updateSelectedSlotAnimation(pos.x - 1, pos.y - 1, getSlotSize(), getSlotSize(), 
									inputs.getVirtualMouse(player.playernum)->draw_cursor);
							}
						}
						else if ( player.shootmode )
						{
							if ( shootmodeSelectedSlotCursor )
							{
								shootmodeSelectedSlotCursor->setDisabled(false);
								bool snapCursor = !inputs.getVirtualMouse(player.playernum)->lastMovementFromController;
								if ( useHotbarFaceMenu && faceMenuSnapCursorInstantly )
								{
									snapCursor = true;
								}
								updateSelectedSlotAnimation(pos.x - 1, pos.y - 1, getSlotSize(), getSlotSize(), snapCursor);
							}
						}
					}
					else
					{
						highlightSlotImg->disabled = true;
					}
				}
			}
			else
			{
				updateSlotFrameFromItem(slotItem, uidToItem(hotbar[num].item));
			}
		}
		else
		{
			updateSlotFrameFromItem(slotItem, uidToItem(hotbar[num].item));
		}
	}
}

bool Player::Hotbar_t::warpMouseToHotbar(const int hotbarSlot, Uint32 flags)
{
	if ( !hotbarFrame || hotbarSlot < 0 || hotbarSlot >= NUM_HOTBAR_SLOTS )
	{
		return false;
	}
	if ( auto slotFrame = getHotbarSlotFrame(hotbarSlot) )
	{
		slotFrame->warpMouseToFrame(player.playernum, flags);
		return true;
	}
	return false;
}

Frame* Player::Hotbar_t::getHotbarSlotFrame(const int hotbarSlot)
{
	if ( !hotbarFrame || hotbarSlot < 0 || hotbarSlot >= NUM_HOTBAR_SLOTS )
	{
		return nullptr;
	}

	char slotname[32];
	snprintf(slotname, sizeof(slotname), "hotbar slot %d", hotbarSlot);
	return hotbarFrame->findFrame(slotname);
}

void doNewCharacterSheet(int player)
{
#ifdef NINTENDO
    static const char* bigfont = "rom://fonts/pixelmix.ttf#18";
    static const char* smallfont = "rom://fonts/pixel_maz.ttf#32";
#else // NINTENDO
    static const char* bigfont = "fonts/pixelmix.ttf#18";
    static const char* smallfont = "fonts/pixel_maz.ttf#14";
#endif // NINTENDO

	return;

	const int w = 200;
	char name[32];
	snprintf(name, sizeof(name), "player charsheet %d", player);
    Frame* frame = gui->findFrame(name);
    if (!frame) {
        frame = gui->addFrame(name);
        frame->setSize(SDL_Rect{
            players[player]->camera_x2() - w,
            players[player]->camera_y1(),
            w, players[player]->camera_height()});
        frame->setColor(makeColor(154, 154, 154, 255));

        // map button
        {
            Button* b = frame->addButton("minimap button");
            b->setFont(smallfont);
            b->setText("Open Map");
            b->setSize(SDL_Rect{0, 8, frame->getSize().w, 40});
            b->setBorder(2);
            b->setBorderColor(makeColor(128, 128, 128, 255));
            b->setColor(makeColor(192, 192, 192, 255));
            b->setTextColor(makeColor(255, 255, 255, 255));
        }

        // log button
        {
            Button* b = frame->addButton("log button");
            b->setFont(smallfont);
            b->setText("Open Log");
            b->setSize(SDL_Rect{0, 56, frame->getSize().w, 40});
            b->setBorder(2);
            b->setBorderColor(makeColor(128, 128, 128, 255));
            b->setColor(makeColor(192, 192, 192, 255));
            b->setTextColor(makeColor(255, 255, 255, 255));
        }

        // game timer
        {

        }

        // 
        {
        }
    } else {
        frame->setDisabled(players[player]->shootmode);
		frame->setSize(SDL_Rect{
			players[player]->camera_x2() - w,
			players[player]->camera_y1(),
			w, players[player]->camera_height() });
    }
}

static Uint32 gui_ticks = 0u;
void doFrames() {
	if ( gui )
	{
		while ( gui_ticks < ticks )
		{
			(void)gui->process();
			++gui_ticks;
		}
		gui->draw();
	}
}