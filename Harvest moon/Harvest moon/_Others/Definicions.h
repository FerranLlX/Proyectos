#pragma once

// Colors
#define C_BLACK	0xFF000000
#define C_RED	0xFFFF0000
#define C_GREEN	0xFF00FF00
#define C_BLUE	0xFF0000FF

#pragma region Technical specifications GameBoyColor

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	Technical specifications																																	//
//																																								//			
//		Size																																					//
//			Approximately 90 mm(3.5 in) x 148 mm(5.8 in) x 32 mm(1.3 in) (WxHxD)[28]																			//
//																																								//
//		Weight																																					//
//			Approximately 220 g(7.8 oz)[29]																														//
//																																								//
//		Screen																																					//
//			2.6 inch reflective super - twisted nematic(STN) liquid - crystal display(LCD)[28]																	//
//			Vertical blank duration : Approximately 1.1 ms[30]																									//
//																																								//
//		Display size																																			//
//			Original : 47 mm(1.9 in) by 43 mm(1.7 in)[29]																										//
//			Pocket : 48 mm(1.9 in) by 44 mm(1.7 in)[29]																											//
//																																								//
//		Framerate																																				//
//			59.727500569606 Hz[31]																																//
//																																								//
//		Power																																					//
//			6 V, 0.7 W(4× AA batteries)[32]																														//
//																																								//
//		Battery life																																			//
//			Approximately 15 hours of gameplay[28]																												//
//																																								//
//		CPU																																						//
//			Custom 8 - bit Sharp LR35902(based on modified 8080 and Z80)[33][34] at 4.19 MHz[b]																	//
//																																								//
//		Memory																																					//
//			64 KiB address space including :																													//
//				.- 8 KiB of built - in working RAM																												//
//				.- Up to sixteen 8 KiB switchable working RAM pages(in the game cartridge) for a maximum of 128 KiB of external									//
//					RAM(which may be battery - backed to hold save games)																						//
//				.- 8 KiB RAM for LCD display																													//
//				.- 32 KiB external Game Pak ROM, of which 16 KiB is switchable																					//
//			On - CPU - Die 256 - byte bootstrap ROM; [35] 32 KiB, 64 KiB, 128 KiB, 256 KiB, 512 KiB, 1 MiB, 2 MiB, 4 MiB and 8 MiB cartridges					//
//																																								//
//		Resolution																																				//
//			160 (w)× 144 (h)pixels(10:9 aspect ratio)																											//
//																																								//
//		Color support	2 - bit(4 shades of "gray": light to very dark olive green)																				//
//			Reference:																																			//
//				.- Original color scheme : 0x0  0x1  0x2  0x3																									//
//				.- Pocket / Light color scheme : 0x0  0x1  0x2  0x3																								//
//																																								//
//		Sound																																					//
//			2 pulse wave generators, 1 PCM 4 - bit wave sample(64 4 - bit samples played in 1×64 bank or 2×32 bank) channel, 1 noise generator,					//
//			and one audio input from the cartridge.[36] The unit has only one speaker, however the headphone port outputs stereo sound.							//
//																																								//
//		Input																																					//
//			Eight - way control pad																																//
//			Four action buttons(A, B, Start, Select)																											//
//			Volume potentiometer																																//
//			Contrast potentiometer																																//
//			Power switch																																		//
//			Serial I / O("Link cable") : 512 kbit / s with up to 4 connections in serial																		//
//			Cartridge I / O																																		//
//																																								//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma endregion

#define FPS	60

#define TILE_W 16
#define TILE_H 16

#define PLAYER_W 16
#define PLAYER_H 32

// Game Boy Color resolution  160×144 pixels.
// TILES (16*16)=	Width 10 tiles	*	Heigth 9 tiles
// In pixels	=	160px			*	144px			== resolution of GBC
#define SIZE_CAMERA_W 160 * 3	// 480
#define SIZE_CAMERA_H 144 * 3	// 432

// WW i WH
#define SIZE_WORLD_W 49 * TILE_W	// 784
#define SIZE_WORLD_H 49 * TILE_H	// 784