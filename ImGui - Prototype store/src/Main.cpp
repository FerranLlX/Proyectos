#include "general_headers.h"
#include "Video.h"
#include "items.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <vector>
#include <string>


using namespace ImGui;
using namespace std;

//Load an IMAGE and return its pointer to manage it.
	// file -> (path) of the file to be uploaded
	// RETURN ImTextureID
ImTextureID addImage(const char* file) {
	GLuint       LoadedTexture = 0;
	int			w, h, bytesPixel;
	unsigned char* pixels = stbi_load(file, &w, &h, &bytesPixel, 0);
	if (!pixels) {
		std::cout << "Error when loading image: " << file << ": stbi_load ERROR." << std::endl;
		return NULL;
	}
	else {
		GLuint newTextureID = NULL;
		glGenTextures(1, &newTextureID);
		glBindTexture(GL_TEXTURE_2D, newTextureID);
		GLint Mode = GL_RGB;
		if (bytesPixel == 4) Mode = GL_RGBA;
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
			std::cerr << "Unable to create texture: Problems with OpenGL." << std::endl;
		}
		return (ImTextureID)(intptr_t)newTextureID;
	}
}

struct Products
{
	char type[10];
	int amount;
	char name[20];
	int priceSell;
	int priceBuy;
	char description[256];
};


void LoadVectorItems(vector<Products>* vectorWithAllInformation, vector<string>* vectorObjectsAll);
void FilterAll(vector<string>* vectorObjectsAll, vector<string>* vectorObjectsShow);
void FilterWeapons(vector<Products>* vectorWithAllInformation, vector<string>* vectorObjectsShow);
void FilterArmors(vector<Products>* vectorWithAllInformation, vector<string>* vectorObjectsShow);
void FilterShields(vector<Products>* vectorWithAllInformation, vector<string>* vectorObjectsShow);
void FilterItems(vector<Products>* vectorWithAllInformation, vector<string>* vectorObjectsShow);
Products LoadInformationItemSelected(int i, Products productsShow, vector<Products>* vectorWithAllInformation);
void FilterAllInventory(vector<Products>* vectorInventory, vector<string>* vectorObjectsShow);
void CheckIdenticalItemAndAddInventory(vector<Products>* vectorInventory, Products* productSelected);
Products LoadInformationInventorySelectedItem(int position, Products vectorObjectsShow, vector<Products>* vectorInventory);
void DeleteItemInventory(vector<Products>* vectorInventory, Products* productSelected, int* totalMoney);


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


#pragma region SHOP

	static bool showBuyShop = true;
	static bool showSellShop = false;
	static bool showImage = true;
	static bool p_open = false;
	static bool showSimpleLayout = false;

	vector<Products>* vectorWithAllInformation = new vector<Products>;
	vector<string>* vectorAllObjects = new vector<string>;
	vector<string>* vectorShowObjects = new vector<string>;
	LoadVectorItems(vectorWithAllInformation, vectorAllObjects);
	vector<Products>* vectorInventory = new vector<Products>;

	Products productsToDisplay;
	productsToDisplay = { "---", 0, "---", 0, 0, "---" };

	float sizeX_Shop = 900;
	float sizeY_Shop = 600;
	float posX_Shop = 100;
	float posY_Shop = 100;

	float sizeX_ListItems = 350;
	float sizeY_ListItems = 250;
	float posX_Shop_ListItems = posX_Shop + ((sizeX_Shop - sizeX_ListItems) / 2);
	float posY_Shop_ListItems = posY_Shop + ((sizeY_Shop - sizeY_ListItems) / 4);

	float sizeX_ImageItem = 350;
	float sizeY_ImageItem = 150;
	float posX_Image = posX_Shop_ListItems - 15;
	float posY_Image = posY_Shop_ListItems + 20 + sizeY_ListItems;

	float sizeX_ListBuy = 220;
	float sizeY_ListBuy = 420;
	float posX_ListBuy = (sizeX_Shop / 6) * 5;
	float posY_ListBuy = posY_Shop_ListItems;

	float sizeX_Money = 150;
	float sizeY_Money = 70;
	float posX_Money = posX_Shop - 10;
	float posY_Money = posY_Shop - 15;

	float sizeX_NameShop = sizeX_Money;
	float sizeY_NameShop = sizeY_Money;
	float posX_NameShop = posX_Shop_ListItems + (sizeX_Money / 3 * 2);
	float posY_NameShop = posY_Money;

	float sizeX_TextBuy = sizeX_Money;
	float sizeY_TextBuy = sizeY_Money;
	float posX_TextBuy = sizeX_Shop + posX_Shop - sizeX_Money + 10;
	float posY_TextBuy = posY_Money;

	float sizeX_Icon = 15;
	float sizeY_Icon = 15;

	// Variables
	static int counter = 0;
	static int moneyTotal = 1000;
	static int priceSelectedObject = 100;

	// Carrega de IMATGE
	ImTextureID IMG_forge = addImage("src/forja.png");
	ImTextureID ICON_health = addImage("src/1_vida.png");
	ImTextureID ICON_damage = addImage("src/2_dany.png");
	ImTextureID ICON_armor = addImage("src/3_armor.png");
	ImTextureID ICON_speed = addImage("src/4_speed.png");
	ImTextureID ICON_money = addImage("src/moneda.png");
	ImTextureID IMG_guy = addImage("src/herrero.png");
	ImTextureID IMG_inverseGuy = addImage("src/herrero2.png");

#pragma endregion

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


#pragma region PROTOTYPE SHOP

		if (showBuyShop)
		{
			SetNextWindowSizeConstraints(ImVec2(sizeX_Shop, sizeY_Shop), ImVec2(sizeX_Shop, sizeY_Shop));
			SetNextWindowPos(ImVec2(posX_Shop, posY_Shop));
			Begin("Shop", &showBuyShop, ImGuiWindowFlags_Personalitzat0_Botiga);
			{
				SetNextWindowSizeConstraints(ImVec2(sizeX_ListItems, sizeY_ListItems), ImVec2(sizeX_ListItems, sizeY_ListItems));
				SetNextWindowPos(ImVec2(posX_Shop_ListItems, posY_Shop_ListItems));
				Begin("ListBuy", &showBuyShop, ImGuiWindowFlags_Personalitzat1_Llista);
				{
					ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
					if (BeginTabBar("OptionsShopBuy", tab_bar_flags))
					{
						if (BeginTabItem("All"))
						{
							Text("List of all objects shop.");
							FilterAll(vectorAllObjects, vectorShowObjects);
							EndTabItem();
						}
						if (BeginTabItem("Weapons"))
						{
							Text("List all weapons.");
							FilterWeapons(vectorWithAllInformation, vectorShowObjects);
							EndTabItem();
						}
						if (BeginTabItem("Armors"))
						{
							Text("List all armors.");
							FilterArmors(vectorWithAllInformation, vectorShowObjects);
							EndTabItem();
						}
						if (BeginTabItem("Shields"))
						{
							Text("List all shields.");
							FilterShields(vectorWithAllInformation, vectorShowObjects);
							EndTabItem();
						}
						if (BeginTabItem("Items"))
						{
							Text("List all items.");
							FilterItems(vectorWithAllInformation, vectorShowObjects);
							EndTabItem();
						}
						EndTabBar();
					}


					{
						int sizeObjects = vectorShowObjects->size();
						for (int i = 0; i < sizeObjects; i++)
						{
							const char* nameTempo = vectorShowObjects->at(i).c_str();

							if (Button(nameTempo, ImVec2(320, 50)))
							{
								productsToDisplay = LoadInformationItemSelected(i, productsToDisplay, vectorWithAllInformation);
							}
						}
					}

					// Image under
					SetNextWindowSizeConstraints(ImVec2(sizeX_ImageItem, sizeY_ImageItem), ImVec2(sizeX_ImageItem, sizeY_ImageItem));
					SetNextWindowPos(ImVec2(posX_Image, posY_Image));
					Begin("Image and button image", &showImage, ImGuiWindowFlags_Personalitzat2);
					Image(IMG_forge, ImVec2(sizeX_ImageItem, sizeY_ImageItem));
					End();
				}
				End();


				SetNextWindowSizeConstraints(ImVec2(sizeX_ListBuy, sizeY_ListBuy), ImVec2(sizeX_ListBuy, sizeY_ListBuy));
				SetNextWindowPos(ImVec2(posX_ListBuy, posY_ListBuy));
				Begin("ListFeaturesBuy", &showBuyShop, ImGuiWindowFlags_Personalitzat3);
				{
					// Icon + sameline + text + textQuantity				
					Begin("Icons", &showImage, ImGuiWindowFlags_Personalitzat2);
					{
						Text("Name selected object:");
						Text("%s", productsToDisplay.name);
						Text("");
						Text("");

						Image(ICON_health, ImVec2(sizeX_Icon, sizeY_Icon));
						SameLine();
						Text("Health 			");
						SameLine();
						Text("	+0");

						Image(ICON_damage, ImVec2(sizeX_Icon, sizeY_Icon));
						SameLine();
						Text("Damage 			");
						SameLine();
						Text("	+0");

						Image(ICON_armor, ImVec2(sizeX_Icon, sizeY_Icon));
						SameLine();
						Text("Armor 		");
						SameLine();
						Text("	+0");

						Image(ICON_speed, ImVec2(sizeX_Icon, sizeY_Icon));
						SameLine();
						Text("Speed		");
						SameLine();
						Text("	+0");


						Text("");
						Text("");
						Text("");
						Text("Description object ....");
						Text(productsToDisplay.description);
						Text("............................");
						Text("............................");
						Text("");
						Text("");
						Text("");
						Text("");



						Text("PRICE: %d", productsToDisplay.priceBuy);
						SameLine();
						Image(ICON_money, ImVec2(sizeX_Icon, sizeY_Icon));


						if (Button("BUY##Item", ImVec2(200, 50)))
						{
							if (!(moneyTotal < productsToDisplay.priceBuy))
							{
								moneyTotal = moneyTotal - productsToDisplay.priceBuy;
								CheckIdenticalItemAndAddInventory(vectorInventory, &productsToDisplay);
							}
						}
					}
					End();
				}
				End();


				// Superior squares
				{
					SetNextWindowSizeConstraints(ImVec2(sizeX_Money, sizeY_Money), ImVec2(sizeX_Money, sizeY_Money));
					SetNextWindowPos(ImVec2(posX_Money, posY_Money));
					Begin("TotalMoney", &showBuyShop, ImGuiWindowFlags_Personalitzat3);
					{
						Text("Money: %d", moneyTotal);
						SameLine();
						Image(ICON_money, ImVec2(sizeX_Icon, sizeY_Icon));
					}
					End();

					SetNextWindowSizeConstraints(ImVec2(sizeX_NameShop, sizeY_NameShop), ImVec2(sizeX_NameShop, sizeY_NameShop));
					SetNextWindowPos(ImVec2(posX_NameShop, posY_NameShop));
					Begin("NameShop", &showBuyShop, ImGuiWindowFlags_Personalitzat3);
					{
						Text("Name shop");
					}
					End();

					SetNextWindowSizeConstraints(ImVec2(sizeX_TextBuy, sizeY_TextBuy), ImVec2(sizeX_TextBuy, sizeY_TextBuy));
					SetNextWindowPos(ImVec2(posX_TextBuy, posY_TextBuy));
					Begin("TextBuy", &showBuyShop, ImGuiWindowFlags_Personalitzat3);
					{
						if (Button("BUY##ChangeToSell", ImVec2(150, 50)))
						{
							showBuyShop = false;
							showSellShop = true;
							productsToDisplay = { "---", 0, "---", 0, 0, "---" };
						}
					}
					End();
				}
			}
			End();
		}

		if (showSellShop)
		{
			SetNextWindowSizeConstraints(ImVec2(sizeX_Shop, sizeY_Shop), ImVec2(sizeX_Shop, sizeY_Shop));
			SetNextWindowPos(ImVec2(posX_Shop, posY_Shop));
			Begin("Shop", &showSellShop, ImGuiWindowFlags_Personalitzat0_Botiga);
			{
				SetNextWindowSizeConstraints(ImVec2(sizeX_ListItems, sizeY_ListItems), ImVec2(sizeX_ListItems, sizeY_ListItems));
				SetNextWindowPos(ImVec2(posX_Shop_ListItems, posY_Shop_ListItems));
				Begin("ListBuy", &showSellShop, ImGuiWindowFlags_Personalitzat1_Llista);
				{
					ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
					if (BeginTabBar("OptionsShopBuy", tab_bar_flags))
					{
						if (BeginTabItem("All"))
						{
							Text("List of all objects of inventory.");
							FilterAllInventory(vectorInventory, vectorShowObjects);
							EndTabItem();
						}

						EndTabBar();
					}

					// Listbox manual
					{
						int sizeObjectes = vectorShowObjects->size();
						for (int i = 0; i < sizeObjectes; i++)
						{
							const char* nomTemo = vectorShowObjects->at(i).c_str();

							if (Button(nomTemo, ImVec2(320, 50)))
							{
								productsToDisplay = LoadInformationInventorySelectedItem(i, productsToDisplay, vectorInventory);
							}
						}
					}

					// Image under
					SetNextWindowSizeConstraints(ImVec2(sizeX_ImageItem, sizeY_ImageItem), ImVec2(sizeX_ImageItem, sizeY_ImageItem));
					SetNextWindowPos(ImVec2(posX_Image, posY_Image));
					Begin("Image and button image", &showImage, ImGuiWindowFlags_Personalitzat2);
					Image(IMG_forge, ImVec2(sizeX_ImageItem, sizeY_ImageItem));
					End();
				}
				End();


				SetNextWindowSizeConstraints(ImVec2(sizeX_ListBuy, sizeY_ListBuy), ImVec2(sizeX_ListBuy, sizeY_ListBuy));
				SetNextWindowPos(ImVec2(posX_ListBuy, posY_ListBuy));
				Begin("ListFeaturesBuy", &showSellShop, ImGuiWindowFlags_Personalitzat3);
				{
					// Icon + sameline + text + textQuantity				
					Begin("Icons", &showImage, ImGuiWindowFlags_Personalitzat2);
					{
						Text("Name selected object:");
						Text("%s", productsToDisplay.name);
						Text("");
						Text("");

						Image(ICON_health, ImVec2(sizeX_Icon, sizeY_Icon));
						SameLine();
						Text("Health 			");
						SameLine();
						Text("	+0");

						Image(ICON_damage, ImVec2(sizeX_Icon, sizeY_Icon));
						SameLine();
						Text("Damage 			");
						SameLine();
						Text("	+0");

						Image(ICON_armor, ImVec2(sizeX_Icon, sizeY_Icon));
						SameLine();
						Text("Armor 		");
						SameLine();
						Text("	+0");

						Image(ICON_speed, ImVec2(sizeX_Icon, sizeY_Icon));
						SameLine();
						Text("Speed		");
						SameLine();
						Text("	+0");


						Text("");
						Text("");
						Text("");
						Text("Description of object ....");
						Text(productsToDisplay.description);
						Text("............................");
						Text("............................");
						Text("");
						Text("");
						Text("");
						Text("");



						Text("PRICE: %d", productsToDisplay.priceSell);
						SameLine();
						Image(ICON_money, ImVec2(sizeX_Icon, sizeY_Icon));


						if (Button("SELL##Item", ImVec2(200, 50)))
						{
							DeleteItemInventory(vectorInventory, &productsToDisplay, &moneyTotal);
						}


					}
					End();
				}
				End();

				// Squares superior	
				{
					SetNextWindowSizeConstraints(ImVec2(sizeX_Money, sizeY_Money), ImVec2(sizeX_Money, sizeY_Money));
					SetNextWindowPos(ImVec2(posX_Money, posY_Money));
					Begin("TextSell", &showSellShop, ImGuiWindowFlags_Personalitzat3);
					{
						if (Button("SELL##ChangeToBuy", ImVec2(150, 50)))
						{
							showBuyShop = true;
							showSellShop = false;
							productsToDisplay = { "---", 0, "---", 0, 0, "---" };
						}

					}
					End();

					SetNextWindowSizeConstraints(ImVec2(sizeX_NameShop, sizeY_NameShop), ImVec2(sizeX_NameShop, sizeY_NameShop));
					SetNextWindowPos(ImVec2(posX_NameShop, posY_NameShop));
					Begin("NameShop", &showSellShop, ImGuiWindowFlags_Personalitzat3);
					{
						Text("Name shop");
					}
					End();

					SetNextWindowSizeConstraints(ImVec2(sizeX_TextBuy, sizeY_TextBuy), ImVec2(sizeX_TextBuy, sizeY_TextBuy));
					SetNextWindowPos(ImVec2(posX_TextBuy, posY_TextBuy));
					Begin("TotalMoney", &showSellShop, ImGuiWindowFlags_Personalitzat3);
					{
						Text("Money: %d", moneyTotal);
						SameLine();
						Image(ICON_money, ImVec2(sizeX_Icon, sizeY_Icon));
					}
					End();
				}
			}
			End();
		}


		if (event.key.keysym.sym == SDLK_F1)
		{
			// BUY
			if (showBuyShop) showBuyShop = false;
			else showBuyShop = true;
		}
		else if (event.key.keysym.sym == SDLK_F2)
		{
			// SELL
			if (showSellShop) showSellShop = false;
			else showSellShop = true;
		}

#pragma endregion


		// Rendering
		ImGui::Render(); // Prepare ImGUI for render. Create VBO and draw data.
		Video->clearBackground(clear_color);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData()); // Real Render to Screen
		Video->SwapBuffer();
	}

	// Remember free all images.
	glDeleteTextures(1, (GLuint*)&IMG_forge);
	glDeleteTextures(1, (GLuint*)&ICON_health);
	glDeleteTextures(1, (GLuint*)&ICON_damage);
	glDeleteTextures(1, (GLuint*)&ICON_armor);
	glDeleteTextures(1, (GLuint*)&ICON_speed);
	glDeleteTextures(1, (GLuint*)&ICON_money);
	glDeleteTextures(1, (GLuint*)&IMG_guy);
	glDeleteTextures(1, (GLuint*)&IMG_inverseGuy);

	// Cleanup
	Video->close();

	return 0;
}

#pragma region SHOP

void LoadVectorItems(vector<Products>* vectorWithAllInformation, vector<string>* vectorAllObjects) {

	Products productTempo;

	productTempo = { "Item", 5, "Chimaera Wing", 70, 35, "Returns you to the castle" };
	vectorWithAllInformation->push_back(productTempo);

	productTempo = { "Item",10,"Dragon scale",20,10,"Increases Defense by 2 when equipped." };
	vectorWithAllInformation->push_back(productTempo);

	productTempo = { "Item",15,"Holy water",40,7,"Keeps enemies away when used outised of battle. Damages enemies if used during a battle." };
	vectorWithAllInformation->push_back(productTempo);

	productTempo = { "Item",3,"Magic Key",100,100,"Single-use key used to a locked door before it breaks." };
	vectorWithAllInformation->push_back(productTempo);

	productTempo = { "Item",20,"Medicinal Herb",25,5,"Restores 15 HP." };
	vectorWithAllInformation->push_back(productTempo);

	productTempo = { "Item",25,"Torch",8,1,"Temporarily illuminates caves. Its radius is lower compared to the Glow spell. Can be chucked to enemies to inflict fire damage." };
	vectorWithAllInformation->push_back(productTempo);

	productTempo = { "Armor",4,"Leather suit",70,30,"Defense +4" };
	vectorWithAllInformation->push_back(productTempo);

	productTempo = { "Armor",2,"Iron armour",1000,700,"Defense +16" };
	vectorWithAllInformation->push_back(productTempo);

	productTempo = { "Armor", 1, "Magic armour", 7700, 5000, "Defense +24" };
	vectorWithAllInformation->push_back(productTempo);

	productTempo = { "Armor",1,"Hero’s armour",10000,6000,"Defense +28" };
	vectorWithAllInformation->push_back(productTempo);

	productTempo = { "Weapon",3,"Oaken club",60,20,"Attack +4" };
	vectorWithAllInformation->push_back(productTempo);

	productTempo = { "Weapon",6,"Copper sword",180,70,"Attack +10" };
	vectorWithAllInformation->push_back(productTempo);

	productTempo = { "Weapon",2,"Hand Axe",560,250,"Attack +15" };
	vectorWithAllInformation->push_back(productTempo);

	productTempo = { "Weapon",1,"Flame Sword",9800,6000,"Attack +28" };
	vectorWithAllInformation->push_back(productTempo);

	productTempo = { "Weapon",0,"Hero’s Sword",20000,15000,"Attack +40" };
	vectorWithAllInformation->push_back(productTempo);

	productTempo = { "Shield",5,"Small Shield",90,95,"Defense +4" };
	vectorWithAllInformation->push_back(productTempo);

	productTempo = { "Shield",2,"Iron Shield",800,350,"Defense +10" };
	vectorWithAllInformation->push_back(productTempo);

	productTempo = { "Shield",1,"Silver Shield",14000,10000,"Defense +20" };
	vectorWithAllInformation->push_back(productTempo);

	int sizeItems = vectorWithAllInformation->size();
	for (int i = 0; i < sizeItems; i++)
	{
		vectorAllObjects->push_back(vectorWithAllInformation->at(i).name);
	}
}

void FilterAll(vector<string>* vectorAllObjects, vector<string>* vectorShowObjects) {

	int sizeShow = vectorShowObjects->size();
	if (sizeShow >= 1)
	{
		for (int i = 0; i < sizeShow; i++)
		{
			vectorShowObjects->erase(vectorShowObjects->begin());
		}
	}

	int sizeAll = vectorAllObjects->size();
	for (int i = 0; i < sizeAll; i++)
	{
		vectorShowObjects->push_back(vectorAllObjects->at(i));
	}
}

void FilterWeapons(vector<Products>* vectorWithAllInformation, vector<string>* vectorShowObjects) {
	int sizeShow = vectorShowObjects->size();
	if (sizeShow >= 1)
	{
		for (int i = 0; i < sizeShow; i++)
		{
			vectorShowObjects->erase(vectorShowObjects->begin());
		}
	}

	int sizeAll = vectorWithAllInformation->size();
	for (int i = 0; i < sizeAll; i++)
	{
		if (strcmp(vectorWithAllInformation->at(i).type, "Weapon") == 0)
		{
			vectorShowObjects->push_back(vectorWithAllInformation->at(i).name);
		}
	}
}

void FilterArmors(vector<Products>* vectorWithAllInformation, vector<string>* vectorShowObjects) {
	int sizeShow = vectorShowObjects->size();
	if (sizeShow >= 1)
	{
		for (int i = 0; i < sizeShow; i++)
		{
			vectorShowObjects->erase(vectorShowObjects->begin());
		}
	}

	int sizeAll = vectorWithAllInformation->size();
	for (int i = 0; i < sizeAll; i++)
	{
		if (strcmp(vectorWithAllInformation->at(i).type, "Armor") == 0)
		{
			vectorShowObjects->push_back(vectorWithAllInformation->at(i).name);
		}
	}
}

void FilterShields(vector<Products>* vectorWithAllInformation, vector<string>* vectorObjectesMostrar) {
	int sizeShow = vectorObjectesMostrar->size();
	if (sizeShow >= 1)
	{
		for (int i = 0; i < sizeShow; i++)
		{
			vectorObjectesMostrar->erase(vectorObjectesMostrar->begin());
		}
	}

	int sizeAll = vectorWithAllInformation->size();
	for (int i = 0; i < sizeAll; i++)
	{
		if (strcmp(vectorWithAllInformation->at(i).type, "Shield") == 0)
		{
			vectorObjectesMostrar->push_back(vectorWithAllInformation->at(i).name);
		}
	}
}

void FilterItems(vector<Products>* vectorWithAllInformation, vector<string>* vectorShowObjects) {
	int sizeShow = vectorShowObjects->size();
	if (sizeShow >= 1)
	{
		for (int i = 0; i < sizeShow; i++)
		{
			vectorShowObjects->erase(vectorShowObjects->begin());
		}
	}

	int sizeAll = vectorWithAllInformation->size();
	for (int i = 0; i < sizeAll; i++)
	{
		if (strcmp(vectorWithAllInformation->at(i).type, "Item") == 0)
		{
			vectorShowObjects->push_back(vectorWithAllInformation->at(i).name);
		}
	}
}

Products LoadInformationItemSelected(int position, Products productShow, vector<Products>* vectorWithAllInformation) {
	return productShow = vectorWithAllInformation->at(position);
}

void FilterAllInventory(vector<Products>* vectorInventory, vector<string>* vectorShowObjects) {

	int sizeShow = vectorShowObjects->size();
	if (sizeShow >= 1)
	{
		for (int i = 0; i < sizeShow; i++)
		{
			vectorShowObjects->erase(vectorShowObjects->begin());
		}
	}

	int sizeAll = vectorInventory->size();
	for (int i = 0; i < sizeAll; i++)
	{
		vectorShowObjects->push_back(vectorInventory->at(i).name);
	}
}

void CheckIdenticalItemAndAddInventory(vector<Products>* vectorInventory, Products* productSelected) {
	int sizeInventory = vectorInventory->size();
	bool found = false;

	if (sizeInventory >= 1) {
		for (int i = 0; i < sizeInventory; i++)
		{
			if (strcmp(vectorInventory->at(i).name, productSelected->name) == 0) {
				vectorInventory->at(i).amount == vectorInventory->at(i).amount + 1;
				found = true;
			}
		}

		if (!found)
		{
			Products productTempo = *productSelected;
			productTempo.amount = 1;
			vectorInventory->push_back(productTempo);
		}
	}
	else {
		Products productTempo = *productSelected;
		productTempo.amount = 1;
		vectorInventory->push_back(productTempo);
	}
}

Products LoadInformationInventorySelectedItem(int position, Products productShow, vector<Products>* vectorInventory) {
	return productShow = vectorInventory->at(position);
}

void DeleteItemInventory(vector<Products>* vectorInventory, Products* productSelected, int* totalMoney) {
	int sizeInventory = vectorInventory->size();

	for (int i = 0; i < sizeInventory; i++)
	{
		if (strcmp(vectorInventory->at(i).name, productSelected->name) == 0) {
			vectorInventory->at(i).amount = vectorInventory->at(i).amount - 1;
			totalMoney = totalMoney + vectorInventory->at(i).priceSell;
			if (vectorInventory->at(i).amount <= 0)
			{
				vectorInventory->erase(vectorInventory->begin() + i);
				break;
			}
		}
	}
}

#pragma endregion