
#include "opcodes.h"
#include "util.h"

void headinghandler(struct SPLHXTEXTDRAW *hxtd, struct stTextdraw *samptd, int reason)
{
	TRACE("headinghandler\n");
	REPOSITION_ON_ATTACH();
	if (!INCAR) {
		return;
	}
	int n[7];
	for (int i = 0; i < 7; i++) {
		n[i] = (360 - gamedata.carheading) - 3 + i;
		if (n[i] < 1) n[i] += 360;
		if (n[i] > 360) n[i] -= 360;
	}
	sprintf(samptd->szText, "%03d %03d %03d [%03d] %03d %03d %03d", n[0], n[1], n[2], n[3], n[4], n[5], n[6]);
	memcpy(samptd->szString, samptd->szText, 50);
}

void removehandler(struct SPLHXTEXTDRAW *hxtd, struct stTextdraw *samptd, int reason)
{
	TRACE("removehandler\n");
	REPOSITION_ON_ATTACH();
}

void boxremovehandler(struct SPLHXTEXTDRAW *hxtd, struct stTextdraw *samptd, int reason)
{
	TRACE("boxremovehandler\n");
	if (reason == TDHANDLER_ATTACH) {
		samptd->byteBox = 0;
		samptd->fX = hxtd->fTargetX;
		samptd->fY = hxtd->fTargetY;
		return;
	}
}

void carspeedtdhandler(struct SPLHXTEXTDRAW *hxtd, struct stTextdraw *samptd, int reason)
{
	TRACE("carspeedtdhandler\n");
	REPOSITION_ON_ATTACH();
	if (!INCAR) {
		return;
	}
	sprintf(&samptd->szText[13], "%d KPH", GROUNDSPEED(gamedata.carspeed));
	memcpy(&samptd->szString[13], &samptd->szText[13], 10);
}

void progressbarpatchhandler(struct SPLHXTEXTDRAW *hxtd, struct stTextdraw *samptd, int reason)
{
	TRACE("progressbarpatchhandler\n");
	REPOSITION_ON_ATTACH();
	samptd->szText[0] = '_';
	samptd->szString[0] = '_';
	if (!INCAR) {
		samptd->dwBoxColor &= 0x00ffffff;
	} else {
		samptd->dwBoxColor |= 0xff000000;
	}
}

void damagebarhandler(struct SPLHXTEXTDRAW *hxtd, struct stTextdraw *samptd, int reason)
{
	TRACE("damagebarhandler\n");
	REPOSITION_ON_ATTACH();
	progressbarpatchhandler(hxtd, samptd, reason);
	if (!INCAR) {
		return;
	}
	samptd->fBoxSizeX = 569.0f + (float) gamedata.carhp * 63.0f / 1000.0f;
}

void damagepatchhandler(struct SPLHXTEXTDRAW *hxtd, struct stTextdraw *samptd, int reason)
{
	TRACE("damagepatchhandler\n");
	REPOSITION_ON_ATTACH();
	if (!INCAR) {
		return;
	}
	sprintf(samptd->szText, "  Health");
	memcpy(samptd->szString, samptd->szText, 12);
}

void destnearesthandler(struct SPLHXTEXTDRAW *hxtd, struct stTextdraw *samptd, int reason, char *destnearstr)
{
	destnearstr[0] = 0;
	TRACE("destnearesthandler\n");
	REPOSITION_ON_ATTACH();
	if (!INCAR) {
		return;
	}
	if (gamedata.missiondistance != -1) {
		sprintf(destnearstr, "~b~Distance~b~ ~w~%d M", gamedata.missiondistance);
		return;
	}
	// rescue!!
	if (strncmp(samptd->szText, "Destination (", 13) == 0) {
		int distance = simplestrval(samptd->szText, 13);
		sprintf(destnearstr, "~b~Distance~b~ ~w~%d M", distance);
		return;
	}
	if (strncmp(samptd->szText, "Nearest Airport (", 17) != 0) {
		return;
	}
	int idx = 18;
	int chars = 0;
	while (true) {
		if (samptd->szText[idx] == 'M') {
			chars = sprintf(destnearstr, "~b~%s ", &(samptd->szText[idx + 8]));
			break;
		}
		idx++;
		if (idx > 23) {
			return;
		}
	}
	if (chars == -1) {
		destnearstr[0] = 0;
		return;
	}
	int srcidx = 16;
	if (samptd->szText[srcidx] != '(') {
		return;
	}
	while (true) {
		char c = samptd->szText[srcidx++];
		destnearstr[chars++] = c;
		if (c == ')' || srcidx > 25) {
			break;
		}
	}
	destnearstr[chars] = 0;
}
