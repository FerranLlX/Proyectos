#include "general_headers.h"
#include "Video.h"
#include "items.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <vector>
#include <string>


using namespace ImGui;
using namespace std;

//Carga una IMATGE i retorna el seu punter per gestionar-la
		// file -> ruta(path) del fitxer a carregar
		// RETORNA ImTextureID
ImTextureID addImage(const char* file) {
	GLuint       LoadedTexture = 0;
	int			w, h, bytesPerPixel;
	//Con esto lo mira en la carpeta.
	unsigned char* pixels = stbi_load(file, &w, &h, &bytesPerPixel, 0);
	if (!pixels) {
		std::cout << "Error when loading image: " << file << ": stbi_load ERROR." << std::endl;
		return NULL;
	}
	else {
		GLuint newTextureID = NULL;
		glGenTextures(1, &newTextureID);
		glBindTexture(GL_TEXTURE_2D, newTextureID);
		GLint Mode = GL_RGB;
		if (bytesPerPixel == 4) Mode = GL_RGBA;
		glTexImage2D(GL_TEXTURE_2D, 0, Mode, w, h, 0, Mode, GL_UNSIGNED_BYTE, pixels);
		glGenerateMipmap(GL_TEXTURE_2D);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glBindTexture(GL_TEXTURE_2D, 0);
		stbi_image_free(pixels);
		if (!newTextureID)
		{
			std::cerr << "Impossible crear textura: Problemes amb OpenGL." << std::endl;
		}
		return (ImTextureID)(intptr_t)newTextureID;
	}
}

struct Productes
{
	char type[10];
	int amount;
	char name[20];
	int priceSell;
	int priceBuy;
	char description[256];
};


void CarregarVectorItems(vector<Productes>* vectorAmbTotaLinformacio, vector<string>* vectorObjectesTot);
void FiltrarTot(vector<string>* vectorObjectesTot, vector<string>* vectorObjectesMostrar);
void FiltrarArmes(vector<Productes>* vectorAmbTotaLinformacio, vector<string>* vectorObjectesMostrar);
void FiltrarArmadures(vector<Productes>* vectorAmbTotaLinformacio, vector<string>* vectorObjectesMostrar);
void FiltrarEscuts(vector<Productes>* vectorAmbTotaLinformacio, vector<string>* vectorObjectesMostrar);
void FiltrarItems(vector<Productes>* vectorAmbTotaLinformacio, vector<string>* vectorObjectesMostrar);
Productes CarregarInformacioItemSelecionat(int i, Productes producteAMostrar, vector<Productes>* vectorAmbTotaLinformacio);
void FiltrarTotInventari(vector<Productes>* vectorInventari, vector<string>* vectorObjectesMostrar);
void ComprovarSiEnTenimUnIgualIAfegirInventari(vector<Productes>* vectorInventari, Productes* producteSelecionat);
Productes CarregarInformacioItemSelecionatInventari(int posicio, Productes producteAMostrar, vector<Productes>* vectorInventari);
void EliminarItemInventari(vector<Productes>* vectorInventari, Productes* producteSelecionat, int* dinersTotals);


// Main code
int main(int argc, char* args[])
{
	int error;
	Video_imGui* Video = Video_imGui::getInstance();

	error = Video->init_SDL_openGL_imGui("Test imGUI", 1280, 720);

	if (error) {
		switch (error) {
		case 1:
			std::cout << "Error: " << SDL_GetError() << std::endl;
			break;
		case 2:
			std::cout << "Failed to initialize OpenGL loader!" << std::endl;
			break;
		default:
			break;
		}
		return error;
	}

	// Our state
	bool show_demo_window = false;
	bool show_another_window = false;
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);



	// ----------------------------- BOTIGA -----------------
	static bool showBotigaComprar = true;
	static bool showBotigaVendre = false;
	static bool showImatge = true;
	static bool p_open = false;
	static bool showSimpleLayout = false;

	vector<Productes>* vectorAmbTotaLinformacio = new vector<Productes>;
	vector<string>* vectorObjectesTot = new vector<string>;
	vector<string>* vectorObjectesMostrar = new vector<string>;
	CarregarVectorItems(vectorAmbTotaLinformacio, vectorObjectesTot);
	vector<Productes>* vectorInventari = new vector<Productes>;

	Productes producteAMostrar;
	producteAMostrar = { "---", 0, "---", 0, 0, "---" };

	// Mida i posicio finestra botiga
	float midaX_Botiga = 900;
	float midaY_Botiga = 600;
	float posX_Botiga = 100;
	float posY_Botiga = 100;

	// Mida i posicio finestra llista items
	float midaX_LlistaItems = 350;
	float midaY_LlistaItems = 250;
	float posX_Botiga_llistaItems = posX_Botiga + ((midaX_Botiga - midaX_LlistaItems) / 2);
	float posY_Botiga_llistaItems = posY_Botiga + ((midaY_Botiga - midaY_LlistaItems) / 4);

	// Mida i posicio imatge	
	float midaX_ImatgeItem = 350;
	float midaY_ImatgeItem = 150;
	float posX_Imatge = posX_Botiga_llistaItems - 15;
	float posY_Imatge = posY_Botiga_llistaItems + 20 + midaY_LlistaItems;

	// Mida i posicio llista caracteristiques/comprar
	float midaX_LlistaComprar = 220;
	float midaY_LlistaComprar = 420;
	float posX_LlistaComprar = (midaX_Botiga / 6) * 5;
	float posY_LlistaComprar = posY_Botiga_llistaItems;

	// Mida i posicio Requadre DINERS
	float midaX_Diners = 150;
	float midaY_Diners = 70;
	float posX_Diners = posX_Botiga - 10;
	float posY_Diners = posY_Botiga - 15;

	// Mida i posicio Requadre NOM_BOTIGA
	float midaX_NomBotiga = midaX_Diners;
	float midaY_NomBotiga = midaY_Diners;
	float posX_NomBotiga = posX_Botiga_llistaItems + (midaX_Diners / 3 * 2);
	float posY_NomBotiga = posY_Diners;

	// Mida i posicio Requadre COMPRAR
	float midaX_TextComprar = midaX_Diners;
	float midaY_TextComprar = midaY_Diners;
	float posX_TextComprar = midaX_Botiga + posX_Botiga - midaX_Diners + 10;
	float posY_TextComprar = posY_Diners;

	// Mida icones
	float midaX_Icona = 15;
	float midaY_Icona = 15;

	// Variables
	static int counter = 0;
	static int dinersTotals = 1000;
	static int preuObjecteSelecionat = 100;

	// Carrega de IMATGE
	ImTextureID IMG_forja = addImage("src/forja.png");
	ImTextureID ICON_vida = addImage("src/1_vida.png");
	ImTextureID ICON_dany = addImage("src/2_dany.png");
	ImTextureID ICON_armor = addImage("src/3_armor.png");
	ImTextureID ICON_speed = addImage("src/4_speed.png");
	ImTextureID ICON_moneda = addImage("src/moneda.png");

	// ----------------------------- BOTIGA -----------------



	// Main loop
	bool done = false;
	while (!done)
	{
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			ImGui_ImplSDL2_ProcessEvent(&event);
			if (event.type == SDL_QUIT)
				done = true;
			if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(Video->getWindow()))
				done = true;
		}

		// 0. Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplSDL2_NewFrame(Video->getWindow());
		ImGui::NewFrame();

		// 1. Show the big demo window
		if (show_demo_window)
			ImGui::ShowDemoWindow(&show_demo_window);

		// 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
		{
			static float f = 0.0f;
			static int counter = 0;
			ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.
			ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
			ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
			ImGui::Checkbox("Another Window", &show_another_window);

			ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
			ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

			if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
				counter++;
			ImGui::SameLine();
			ImGui::Text("counter = %d", counter);
			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			ImGui::End();
		}

		// 3. Show another simple window.
		if (show_another_window)
		{
			ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
			ImGui::Text("Hello from another window!");
			if (ImGui::Button("Close Me"))
				show_another_window = false;
			ImGui::End();
		}





		// ------------------------------ BOTIGA ---------------------------------

		if (showBotigaComprar)
		{
			SetNextWindowSizeConstraints(ImVec2(midaX_Botiga, midaY_Botiga), ImVec2(midaX_Botiga, midaY_Botiga));
			SetNextWindowPos(ImVec2(posX_Botiga, posY_Botiga));
			Begin("Botiga", &showBotigaComprar, ImGuiWindowFlags_Personalitzat0_Botiga);
			{
				SetNextWindowSizeConstraints(ImVec2(midaX_LlistaItems, midaY_LlistaItems), ImVec2(midaX_LlistaItems, midaY_LlistaItems));
				SetNextWindowPos(ImVec2(posX_Botiga_llistaItems, posY_Botiga_llistaItems));
				Begin("LlistaComprar", &showBotigaComprar, ImGuiWindowFlags_Personalitzat1_Llista);
				{
					ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
					if (BeginTabBar("OpcionsBotigaComprar", tab_bar_flags))
					{
						if (BeginTabItem("Tot"))
						{
							Text("Llista de tots els objectes de la botiga.");
							FiltrarTot(vectorObjectesTot, vectorObjectesMostrar);
							EndTabItem();
						}
						if (BeginTabItem("Armes"))
						{
							Text("Llista de totes les armes.");
							FiltrarArmes(vectorAmbTotaLinformacio, vectorObjectesMostrar);
							EndTabItem();
						}
						if (BeginTabItem("Armadures"))
						{
							Text("Llista de totes les armadures.");
							FiltrarArmadures(vectorAmbTotaLinformacio, vectorObjectesMostrar);
							EndTabItem();
						}
						if (BeginTabItem("Escuts"))
						{
							Text("Llista de tots els escuts.");
							FiltrarEscuts(vectorAmbTotaLinformacio, vectorObjectesMostrar);
							EndTabItem();
						}
						if (BeginTabItem("Items"))
						{
							Text("Llista de tots els items.");
							FiltrarItems(vectorAmbTotaLinformacio, vectorObjectesMostrar);
							EndTabItem();
						}
						EndTabBar();
					}


					{
						int sizeObjectes = vectorObjectesMostrar->size();
						for (int i = 0; i < sizeObjectes; i++)
						{
							const char* nomTemo = vectorObjectesMostrar->at(i).c_str();

							if (Button(nomTemo, ImVec2(320, 50)))
							{
								producteAMostrar = CarregarInformacioItemSelecionat(i, producteAMostrar, vectorAmbTotaLinformacio);
							}
						}
					}

					// Imatge de sota
					SetNextWindowSizeConstraints(ImVec2(midaX_ImatgeItem, midaY_ImatgeItem), ImVec2(midaX_ImatgeItem, midaY_ImatgeItem));
					SetNextWindowPos(ImVec2(posX_Imatge, posY_Imatge));
					Begin("Imatge i button imatge", &showImatge, ImGuiWindowFlags_Personalitzat2);
					Image(IMG_forja, ImVec2(midaX_ImatgeItem, midaY_ImatgeItem));
					End();
				}
				End();


				SetNextWindowSizeConstraints(ImVec2(midaX_LlistaComprar, midaY_LlistaComprar), ImVec2(midaX_LlistaComprar, midaY_LlistaComprar));
				SetNextWindowPos(ImVec2(posX_LlistaComprar, posY_LlistaComprar));
				Begin("LlistaCaracteristiquesComprar", &showBotigaComprar, ImGuiWindowFlags_Personalitzat3);
				{
					// Icona + sameline + text + textQuantitat				
					Begin("Icones", &showImatge, ImGuiWindowFlags_Personalitzat2);
					{
						Text("Nom objecte selecionat:");
						Text("%s", producteAMostrar.name);
						Text("");
						Text("");

						Image(ICON_vida, ImVec2(midaX_Icona, midaY_Icona));
						SameLine();
						Text("Vida 			");
						SameLine();
						Text("	+0");

						Image(ICON_dany, ImVec2(midaX_Icona, midaY_Icona));
						SameLine();
						Text("Dany 			");
						SameLine();
						Text("	+0");

						Image(ICON_armor, ImVec2(midaX_Icona, midaY_Icona));
						SameLine();
						Text("Armadura 		");
						SameLine();
						Text("	+0");

						Image(ICON_speed, ImVec2(midaX_Icona, midaY_Icona));
						SameLine();
						Text("Velocitat		");
						SameLine();
						Text("	+0");


						Text("");
						Text("");
						Text("");
						Text("Descripcio de l'objecte ....");
						Text(producteAMostrar.description);
						Text("............................");
						Text("............................");
						Text("");
						Text("");
						Text("");
						Text("");



						Text("PREU: %d", producteAMostrar.priceBuy);
						SameLine();
						Image(ICON_moneda, ImVec2(midaX_Icona, midaY_Icona));


						if (Button("COMPRAR##Item", ImVec2(200, 50)))
						{
							if (!(dinersTotals < producteAMostrar.priceBuy))
							{
								dinersTotals = dinersTotals - producteAMostrar.priceBuy;
								ComprovarSiEnTenimUnIgualIAfegirInventari(vectorInventari, &producteAMostrar);
							}
						}
					}
					End();
				}
				End();


				// REQUADRES SUPERIORS					
				{
					SetNextWindowSizeConstraints(ImVec2(midaX_Diners, midaY_Diners), ImVec2(midaX_Diners, midaY_Diners));
					SetNextWindowPos(ImVec2(posX_Diners, posY_Diners));
					Begin("DinersTotals", &showBotigaComprar, ImGuiWindowFlags_Personalitzat3);
					{
						Text("Diners: %d", dinersTotals);
						SameLine();
						Image(ICON_moneda, ImVec2(midaX_Icona, midaY_Icona));
					}
					End();

					SetNextWindowSizeConstraints(ImVec2(midaX_NomBotiga, midaY_NomBotiga), ImVec2(midaX_NomBotiga, midaY_NomBotiga));
					SetNextWindowPos(ImVec2(posX_NomBotiga, posY_NomBotiga));
					Begin("NomBotiga", &showBotigaComprar, ImGuiWindowFlags_Personalitzat3);
					{
						Text("Nom botiga");
					}
					End();

					SetNextWindowSizeConstraints(ImVec2(midaX_TextComprar, midaY_TextComprar), ImVec2(midaX_TextComprar, midaY_TextComprar));
					SetNextWindowPos(ImVec2(posX_TextComprar, posY_TextComprar));
					Begin("TextComprar", &showBotigaComprar, ImGuiWindowFlags_Personalitzat3);
					{
						if (Button("COMPRAR##CanviAVendre", ImVec2(150, 50)))
						{
							showBotigaComprar = false;
							showBotigaVendre = true;
							producteAMostrar = { "---", 0, "---", 0, 0, "---" };
						}

					}
					End();
				}
			}
			End();
		}

		if (showBotigaVendre)
		{
			SetNextWindowSizeConstraints(ImVec2(midaX_Botiga, midaY_Botiga), ImVec2(midaX_Botiga, midaY_Botiga));
			SetNextWindowPos(ImVec2(posX_Botiga, posY_Botiga));
			Begin("Botiga", &showBotigaVendre, ImGuiWindowFlags_Personalitzat0_Botiga);
			{
				SetNextWindowSizeConstraints(ImVec2(midaX_LlistaItems, midaY_LlistaItems), ImVec2(midaX_LlistaItems, midaY_LlistaItems));
				SetNextWindowPos(ImVec2(posX_Botiga_llistaItems, posY_Botiga_llistaItems));
				Begin("LlistaComprar", &showBotigaVendre, ImGuiWindowFlags_Personalitzat1_Llista);
				{
					ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
					if (BeginTabBar("OpcionsBotigaComprar", tab_bar_flags))
					{
						if (BeginTabItem("Tot"))
						{
							Text("Llista de tots els objectes de l'inventari.");
							FiltrarTotInventari(vectorInventari, vectorObjectesMostrar);
							EndTabItem();
						}

						EndTabBar();
					}

					// Listbox manual
					{
						int sizeObjectes = vectorObjectesMostrar->size();
						for (int i = 0; i < sizeObjectes; i++)
						{
							const char* nomTemo = vectorObjectesMostrar->at(i).c_str();

							if (Button(nomTemo, ImVec2(320, 50)))
							{
								producteAMostrar = CarregarInformacioItemSelecionatInventari(i, producteAMostrar, vectorInventari);
							}
						}
					}

					// Imatge de sota
					SetNextWindowSizeConstraints(ImVec2(midaX_ImatgeItem, midaY_ImatgeItem), ImVec2(midaX_ImatgeItem, midaY_ImatgeItem));
					SetNextWindowPos(ImVec2(posX_Imatge, posY_Imatge));
					Begin("Imatge i button imatge", &showImatge, ImGuiWindowFlags_Personalitzat2);
					Image(IMG_forja, ImVec2(midaX_ImatgeItem, midaY_ImatgeItem));
					End();
				}
				End();


				SetNextWindowSizeConstraints(ImVec2(midaX_LlistaComprar, midaY_LlistaComprar), ImVec2(midaX_LlistaComprar, midaY_LlistaComprar));
				SetNextWindowPos(ImVec2(posX_LlistaComprar, posY_LlistaComprar));
				Begin("LlistaCaracteristiquesComprar", &showBotigaVendre, ImGuiWindowFlags_Personalitzat3);
				{
					// Icona + sameline + text + textQuantitat				
					Begin("Icones", &showImatge, ImGuiWindowFlags_Personalitzat2);
					{
						Text("Nom objecte selecionat:");
						Text("%s", producteAMostrar.name);
						Text("");
						Text("");

						Image(ICON_vida, ImVec2(midaX_Icona, midaY_Icona));
						SameLine();
						Text("Vida 			");
						SameLine();
						Text("	+0");

						Image(ICON_dany, ImVec2(midaX_Icona, midaY_Icona));
						SameLine();
						Text("Dany 			");
						SameLine();
						Text("	+0");

						Image(ICON_armor, ImVec2(midaX_Icona, midaY_Icona));
						SameLine();
						Text("Armadura 		");
						SameLine();
						Text("	+0");

						Image(ICON_speed, ImVec2(midaX_Icona, midaY_Icona));
						SameLine();
						Text("Velocitat		");
						SameLine();
						Text("	+0");


						Text("");
						Text("");
						Text("");
						Text("Descripcio de l'objecte ....");
						Text(producteAMostrar.description);
						Text("............................");
						Text("............................");
						Text("");
						Text("");
						Text("");
						Text("");



						Text("PREU: %d", producteAMostrar.priceSell);
						SameLine();
						Image(ICON_moneda, ImVec2(midaX_Icona, midaY_Icona));


						if (Button("VENDRE##Item", ImVec2(200, 50)))
						{
							EliminarItemInventari(vectorInventari, &producteAMostrar, &dinersTotals);
						}


					}
					End();
				}
				End();

				// REQUADRES SUPERIORS	
				{
					SetNextWindowSizeConstraints(ImVec2(midaX_Diners, midaY_Diners), ImVec2(midaX_Diners, midaY_Diners));
					SetNextWindowPos(ImVec2(posX_Diners, posY_Diners));
					Begin("TextVendre", &showBotigaVendre, ImGuiWindowFlags_Personalitzat3);
					{
						if (Button("VENDRE##CanviAComprar", ImVec2(150, 50)))
						{
							showBotigaComprar = true;
							showBotigaVendre = false;
							producteAMostrar = { "---", 0, "---", 0, 0, "---" };
						}

					}
					End();

					SetNextWindowSizeConstraints(ImVec2(midaX_NomBotiga, midaY_NomBotiga), ImVec2(midaX_NomBotiga, midaY_NomBotiga));
					SetNextWindowPos(ImVec2(posX_NomBotiga, posY_NomBotiga));
					Begin("NomBotiga", &showBotigaVendre, ImGuiWindowFlags_Personalitzat3);
					{
						Text("Nom botiga");
					}
					End();

					SetNextWindowSizeConstraints(ImVec2(midaX_TextComprar, midaY_TextComprar), ImVec2(midaX_TextComprar, midaY_TextComprar));
					SetNextWindowPos(ImVec2(posX_TextComprar, posY_TextComprar));
					Begin("DinersTotals", &showBotigaVendre, ImGuiWindowFlags_Personalitzat3);
					{
						Text("Diners: %d", dinersTotals);
						SameLine();
						Image(ICON_moneda, ImVec2(midaX_Icona, midaY_Icona));
					}
					End();
				}
			}
			End();
		}


		if (event.key.keysym.sym == SDLK_F1)
		{
			// COMPRAR
			if (showBotigaComprar) showBotigaComprar = false;
			else showBotigaComprar = true;
		}
		else if (event.key.keysym.sym == SDLK_F2)
		{
			// VENDRE
			if (showBotigaVendre) showBotigaVendre = false;
			else showBotigaVendre = true;
		}


		// ------------------------------ BOTIGA ----------------------------------




		// Rendering
		ImGui::Render(); // Prepare ImGUI for render. Create VBO and draw data.
		Video->clearBackground(clear_color);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData()); // Real Render to Screen
		Video->SwapBuffer();
	}

	// Recorda alliberar la imatge al acabar el programa
	glDeleteTextures(1, (GLuint*)&IMG_forja);
	glDeleteTextures(1, (GLuint*)&ICON_vida);
	glDeleteTextures(1, (GLuint*)&ICON_dany);
	glDeleteTextures(1, (GLuint*)&ICON_armor);
	glDeleteTextures(1, (GLuint*)&ICON_speed);
	glDeleteTextures(1, (GLuint*)&ICON_moneda);

	// Cleanup
	Video->close();

	return 0;
}


void CarregarVectorItems(vector<Productes>* vectorAmbTotaLinformacio, vector<string>* vectorObjectesTot) {

	Productes producteTempo;

	producteTempo = { "Item", 5, "Chimaera Wing", 70, 35, "Returns you to the castle" };
	vectorAmbTotaLinformacio->push_back(producteTempo);

	producteTempo = { "Item",10,"Dragon scale",20,10,"Increases Defense by 2 when equipped." };
	vectorAmbTotaLinformacio->push_back(producteTempo);

	producteTempo = { "Item",15,"Holy water",40,7,"Keeps enemies away when used outised of battle. Damages enemies if used during a battle." };
	vectorAmbTotaLinformacio->push_back(producteTempo);

	producteTempo = { "Item",3,"Magic Key",100,100,"Single-use key used to a locked door before it breaks." };
	vectorAmbTotaLinformacio->push_back(producteTempo);

	producteTempo = { "Item",20,"Medicinal Herb",25,5,"Restores 15 HP." };
	vectorAmbTotaLinformacio->push_back(producteTempo);

	producteTempo = { "Item",25,"Torch",8,1,"Temporarily illuminates caves. Its radius is lower compared to the Glow spell. Can be chucked to enemies to inflict fire damage." };
	vectorAmbTotaLinformacio->push_back(producteTempo);

	producteTempo = { "Armor",4,"Leather suit",70,30,"Defense +4" };
	vectorAmbTotaLinformacio->push_back(producteTempo);

	producteTempo = { "Armor",2,"Iron armour",1000,700,"Defense +16" };
	vectorAmbTotaLinformacio->push_back(producteTempo);

	producteTempo = { "Armor", 1, "Magic armour", 7700, 5000, "Defense +24" };
	vectorAmbTotaLinformacio->push_back(producteTempo);

	producteTempo = { "Armor",1,"Hero’s armour",10000,6000,"Defense +28" };
	vectorAmbTotaLinformacio->push_back(producteTempo);

	producteTempo = { "Weapon",3,"Oaken club",60,20,"Attack +4" };
	vectorAmbTotaLinformacio->push_back(producteTempo);

	producteTempo = { "Weapon",6,"Copper sword",180,70,"Attack +10" };
	vectorAmbTotaLinformacio->push_back(producteTempo);

	producteTempo = { "Weapon",2,"Hand Axe",560,250,"Attack +15" };
	vectorAmbTotaLinformacio->push_back(producteTempo);

	producteTempo = { "Weapon",1,"Flame Sword",9800,6000,"Attack +28" };
	vectorAmbTotaLinformacio->push_back(producteTempo);

	producteTempo = { "Weapon",0,"Hero’s Sword",20000,15000,"Attack +40" };
	vectorAmbTotaLinformacio->push_back(producteTempo);

	producteTempo = { "Shield",5,"Small Shield",90,95,"Defense +4" };
	vectorAmbTotaLinformacio->push_back(producteTempo);

	producteTempo = { "Shield",2,"Iron Shield",800,350,"Defense +10" };
	vectorAmbTotaLinformacio->push_back(producteTempo);

	producteTempo = { "Shield",1,"Silver Shield",14000,10000,"Defense +20" };
	vectorAmbTotaLinformacio->push_back(producteTempo);

	int sizeItems = vectorAmbTotaLinformacio->size();
	for (int i = 0; i < sizeItems; i++)
	{
		vectorObjectesTot->push_back(vectorAmbTotaLinformacio->at(i).name);
	}
}


void FiltrarTot(vector<string>* vectorObjectesTot, vector<string>* vectorObjectesMostrar) {

	int sizeMostrar = vectorObjectesMostrar->size();
	if (sizeMostrar >= 1)
	{
		for (int i = 0; i < sizeMostrar; i++)
		{
			vectorObjectesMostrar->erase(vectorObjectesMostrar->begin());
		}
	}

	int sizeTot = vectorObjectesTot->size();
	for (int i = 0; i < sizeTot; i++)
	{
		vectorObjectesMostrar->push_back(vectorObjectesTot->at(i));
	}
}


void FiltrarArmes(vector<Productes>* vectorAmbTotaLinformacio, vector<string>* vectorObjectesMostrar) {
	int sizeMostrar = vectorObjectesMostrar->size();
	if (sizeMostrar >= 1)
	{
		for (int i = 0; i < sizeMostrar; i++)
		{
			vectorObjectesMostrar->erase(vectorObjectesMostrar->begin());
		}
	}

	int sizeTot = vectorAmbTotaLinformacio->size();
	for (int i = 0; i < sizeTot; i++)
	{
		if (strcmp(vectorAmbTotaLinformacio->at(i).type, "Weapon") == 0)
		{
			vectorObjectesMostrar->push_back(vectorAmbTotaLinformacio->at(i).name);
		}
	}
}


void FiltrarArmadures(vector<Productes>* vectorAmbTotaLinformacio, vector<string>* vectorObjectesMostrar) {
	int sizeMostrar = vectorObjectesMostrar->size();
	if (sizeMostrar >= 1)
	{
		for (int i = 0; i < sizeMostrar; i++)
		{
			vectorObjectesMostrar->erase(vectorObjectesMostrar->begin());
		}
	}

	int sizeTot = vectorAmbTotaLinformacio->size();
	for (int i = 0; i < sizeTot; i++)
	{
		if (strcmp(vectorAmbTotaLinformacio->at(i).type, "Armor") == 0)
		{
			vectorObjectesMostrar->push_back(vectorAmbTotaLinformacio->at(i).name);
		}
	}
}


void FiltrarEscuts(vector<Productes>* vectorAmbTotaLinformacio, vector<string>* vectorObjectesMostrar) {
	int sizeMostrar = vectorObjectesMostrar->size();
	if (sizeMostrar >= 1)
	{
		for (int i = 0; i < sizeMostrar; i++)
		{
			vectorObjectesMostrar->erase(vectorObjectesMostrar->begin());
		}
	}

	int sizeTot = vectorAmbTotaLinformacio->size();
	for (int i = 0; i < sizeTot; i++)
	{
		if (strcmp(vectorAmbTotaLinformacio->at(i).type, "Shield") == 0)
		{
			vectorObjectesMostrar->push_back(vectorAmbTotaLinformacio->at(i).name);
		}
	}
}


void FiltrarItems(vector<Productes>* vectorAmbTotaLinformacio, vector<string>* vectorObjectesMostrar) {
	int sizeMostrar = vectorObjectesMostrar->size();
	if (sizeMostrar >= 1)
	{
		for (int i = 0; i < sizeMostrar; i++)
		{
			vectorObjectesMostrar->erase(vectorObjectesMostrar->begin());
		}
	}

	int sizeTot = vectorAmbTotaLinformacio->size();
	for (int i = 0; i < sizeTot; i++)
	{
		if (strcmp(vectorAmbTotaLinformacio->at(i).type, "Item") == 0)
		{
			vectorObjectesMostrar->push_back(vectorAmbTotaLinformacio->at(i).name);
		}
	}
}


Productes CarregarInformacioItemSelecionat(int posicio, Productes producteAMostrar, vector<Productes>* vectorAmbTotaLinformacio) {
	return producteAMostrar = vectorAmbTotaLinformacio->at(posicio);
}


void FiltrarTotInventari(vector<Productes>* vectorInventari, vector<string>* vectorObjectesMostrar) {

	int sizeMostrar = vectorObjectesMostrar->size();
	if (sizeMostrar >= 1)
	{
		for (int i = 0; i < sizeMostrar; i++)
		{
			vectorObjectesMostrar->erase(vectorObjectesMostrar->begin());
		}
	}

	int sizeTot = vectorInventari->size();
	for (int i = 0; i < sizeTot; i++)
	{
		vectorObjectesMostrar->push_back(vectorInventari->at(i).name);
	}
}


void ComprovarSiEnTenimUnIgualIAfegirInventari(vector<Productes>* vectorInventari, Productes* producteSelecionat) {
	int sizeInventari = vectorInventari->size();
	bool trobat = false;

	if (sizeInventari >= 1) {
		for (int i = 0; i < sizeInventari; i++)
		{
			if (strcmp(vectorInventari->at(i).name, producteSelecionat->name) == 0) {
				vectorInventari->at(i).amount == vectorInventari->at(i).amount + 1;
				trobat = true;
			}
		}

		if (!trobat)
		{
			Productes producteTempo = *producteSelecionat;
			producteTempo.amount = 1;
			vectorInventari->push_back(producteTempo);
		}

	}
	else {
		Productes producteTempo = *producteSelecionat;
		producteTempo.amount = 1;
		vectorInventari->push_back(producteTempo);
	}
}


Productes CarregarInformacioItemSelecionatInventari(int posicio, Productes producteAMostrar, vector<Productes>* vectorInventari) {
	return producteAMostrar = vectorInventari->at(posicio);
}


void EliminarItemInventari(vector<Productes>* vectorInventari, Productes* producteSelecionat, int* dinersTotals) {
	int sizeInventari = vectorInventari->size();

	for (int i = 0; i < sizeInventari; i++)
	{
		if (strcmp(vectorInventari->at(i).name, producteSelecionat->name) == 0) {
			vectorInventari->at(i).amount = vectorInventari->at(i).amount - 1;
			dinersTotals = dinersTotals + vectorInventari->at(i).priceSell;
			if (vectorInventari->at(i).amount <= 0)
			{
				vectorInventari->erase(vectorInventari->begin() + i);
				break;
			}
		}
	}

}
