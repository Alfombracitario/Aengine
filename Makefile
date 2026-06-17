.SUFFIXES:

PLATFORM ?= WII
DEBUG    ?= 0

.PHONY: all clean run

all:
	@$(MAKE) -f Makefile.$(PLATFORM) PLATFORM=$(PLATFORM)

clean:
	@$(MAKE) -f Makefile.$(PLATFORM) PLATFORM=$(PLATFORM) clean

run:
	@$(MAKE) -f Makefile.$(PLATFORM) PLATFORM=$(PLATFORM) run

# ─── Shortcuts ───────────────────────────────────────────────────────────────
wii:
	@$(MAKE) -f Makefile.WII PLATFORM=WII

wii-clean:
	@$(MAKE) -f Makefile.WII PLATFORM=WII clean

gc:
	@$(MAKE) -f Makefile.GC PLATFORM=GC

gc-clean:
	@$(MAKE) -f Makefile.GC PLATFORM=GC clean

snes:
	@$(MAKE) -f Makefile.SNES PLATFORM=SNES

snes-clean:
	@$(MAKE) -f Makefile.SNES PLATFORM=SNES clean

gba:
	@$(MAKE) -f Makefile.GBA PLATFORM=GBA

gba-clean:
	@$(MAKE) -f Makefile.GBA PLATFORM=GBA clean