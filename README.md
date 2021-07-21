# **N(cursed) Text Editor**
Command line text editor, vim inspired.

After compiled, it can be used as follows:
```
./smk-cursed       -- when ommited, will save as 'untitled'
./smk-cursed <filename>       
```

Supported modes:
```
Normal: as in Vim, currently only works as navigation.
Insert: as any other text editor.
```
Keybinds:
```
'i': when in Normal mode, it will put you in Insert mode.
'x': when in Normal mode, it will exit, without saving what you have changed.
's': when in Normal mode, it will save the changes you made.
ESC: when in Insert mode, it will put you in Normal mode.
```

Road map:
- [x] Files
- [x] Colors (ncurses)
- [x] Normal and Insert mode
- [x] Vim navigation
- [ ] Line numbering
- [ ] Line wrap
- [ ] Search in file
- [ ] Config file
- [ ] Color schemes
- [ ] Animated status line :^)
- [ ] Tab completion
