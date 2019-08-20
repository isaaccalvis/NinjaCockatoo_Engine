#include "Application.h"
#include "ModuleGUI.h"

#include "imgui\imgui.h"
#include "imgui\imgui_impl_opengl2.h"
#include "imgui\imgui_impl_sdl.h"

ModuleGUI::ModuleGUI(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	name = "ModuleGUI";
}

bool ModuleGUI::Init(JSON_Object* root_object)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	ImGui::StyleColorsDark();

	ImGui_ImplSDL2_InitForOpenGL(App->window->window, App->renderer3D->context);
	ImGui_ImplOpenGL2_Init();

	SDL_GetVersion(&sdl_version);

	return true;
}

update_status ModuleGUI::Update(float dt)
{
	ImGui_ImplOpenGL2_NewFrame();
	ImGui_ImplSDL2_NewFrame(App->window->window);
	ImGui::NewFrame();

	if (guiWindows[GUI_TOPBAR])
		if (!GUI_TopBar())
			return update_status::UPDATE_STOP;
	if (guiWindows[GUI_ABOUT])
		if (!GUI_AboutWindow())
			return update_status::UPDATE_STOP;
	if (guiWindows[GUI_CONFIGURATION])
		if (!GUI_ConfigurationWindow())
			return update_status::UPDATE_STOP;

	ImGui::Render();
	ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());

	return UPDATE_CONTINUE;
}

bool ModuleGUI::Save(JSON_Object* root_object)
{
	return true;
}

bool ModuleGUI::Load(JSON_Object* root_object)
{
	return true;
}

bool ModuleGUI::GUI_TopBar()
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Quit"))
			{
				return false;
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("View"))
		{
			if (ImGui::MenuItem("Configuration"))
				guiWindows[GUI_CONFIGURATION] = !guiWindows[GUI_CONFIGURATION];
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Help"))
		{
			if (ImGui::MenuItem("Documentation"))
			{
				ShellExecuteA(NULL, "open", "https://github.com/isaaccalvis/Socialist_Engine/wiki", NULL, NULL, SW_SHOWNORMAL);
			}
			if (ImGui::MenuItem("Download Last"))
			{
				ShellExecuteA(NULL, "open", "https://github.com/isaaccalvis/Socialist_Engine/releases", NULL, NULL, SW_SHOWNORMAL);
			}
			if (ImGui::MenuItem("Report Bug"))
			{
				ShellExecuteA(NULL, "open", "https://github.com/isaaccalvis/Socialist_Engine/issues", NULL, NULL, SW_SHOWNORMAL);
			}
			if (ImGui::MenuItem("About"))
				guiWindows[GUI_ABOUT] = !guiWindows[GUI_ABOUT];

			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
	return true;
}

bool ModuleGUI::GUI_AboutWindow()
{
	ImGui::Begin("About", &guiWindows[GUI_WINDOWS::GUI_ABOUT], ImGuiWindowFlags_NoFocusOnAppearing);
	ImGui::Text("Socialist Engine is a video game engine made by students\nfrom CITM(UPC) at VideoGame Development & Design degree");
	ImGui::Text("Made by Isaac Calvis & Aitor Velez");


	ImGui::Text("License: MIT (More info at License file or at ReadMe)");
	ImGui::End();
	return true;
}

bool ModuleGUI::GUI_ConfigurationWindow()
{
	ImGui::Begin("Configuration", &guiWindows[GUI_WINDOWS::GUI_CONFIGURATION], ImGuiWindowFlags_NoFocusOnAppearing);

	if (ImGui::BeginMenu("Options"))
	{
		//ImGui::MenuItem("Set Defaults");
		if (ImGui::MenuItem("Load"))
		{
			App->LoadModulesInfo();
		}
		if (ImGui::MenuItem("Save"))
		{
			App->SaveModulesInfo();
		}
		ImGui::EndMenu();
	}

	if (ImGui::CollapsingHeader("Application"))
	{
		static char gui_appname[256];
		strcpy_s(gui_appname, 256, App->window->winTitle);
		if (ImGui::InputText("App Name", App->window->winTitle, 256, ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll))
		{
			App->window->SetTitle(gui_appname);
		}
	}

	if (ImGui::CollapsingHeader("Window"))
	{

	}

	if (ImGui::CollapsingHeader("File System"))
	{

	}

	if (ImGui::CollapsingHeader("Input"))
	{
		ImGui::SliderFloat("Camera Speed: ", &App->camera->camera_mov_speed, 1.0f, 10.0f);
		ImGui::SliderFloat("Mouse Wheel Speed: ", &App->camera->mouse_wheel_speed, 0.1f, 5.0f);
		ImGui::SliderFloat("Mouse Sensitivity: ", &App->camera->mouse_sensitivity, 0.01f, 2.0f);

		ImGui::Separator();

		ImGui::Text("Mouse Position: ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(255, 255, 0, 255), "%i, %i", App->input->GetMouseX(), App->input->GetMouseY());

		ImGui::Text("Mouse Motion: ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(255, 255, 0, 255), "%i, %i", App->input->GetMouseXMotion(), App->input->GetMouseYMotion());

		ImGui::Text("Mouse Wheel: ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(255, 255, 0, 255), "%i", App->input->GetMouseZ());

		for (int i = 0; i < 300 /*max keys*/; i++)
		{
			switch (App->input->GetKey(i))
			{
				case KEY_STATE::KEY_DOWN: ImGui::Text("Key Down: %i", i); break;
				case KEY_STATE::KEY_REPEAT: ImGui::Text("Key Repeat: %i", i); break;
				case KEY_STATE::KEY_UP: ImGui::Text("Key UP: %i", i); break;
			}
		}
	}

	if (ImGui::CollapsingHeader("Hardware"))
	{
		static ModuleHardware::hw_info hw_info = App->hardware->GetInfo();
		
		ImGui::Text("SDL_Version: ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(255, 255, 0, 255),"%i.%i.%i" , sdl_version.major, sdl_version.minor, sdl_version.patch);

		ImGui::Separator();

		ImGui::Text("CPUs: ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(255,255,0,255), "%i (Cache: %ikb)", hw_info.cpu_count, SDL_GetCPUCacheLineSize());
		
		ImGui::Text("RAM: ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(255, 255, 0, 255), "%iGb", SDL_GetSystemRAM() / 1024);
		
		ImGui::Text("Caps:");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(255, 255, 0, 255), "%s%s%s%s%s%s",
			hw_info.rdtsc ? "RDTSC," : "",
			hw_info.altivec ? "AltiVec," : "",
			hw_info.mmx ? "MMX," : "",
			hw_info.now3d ? "3DNow," : "",
			hw_info.sse ? "SSE," : "",
			hw_info.sse2 ? "SSE2," : "");
		ImGui::TextColored(ImVec4(255, 255, 0, 255), "%s%s%s%s%s",
			hw_info.sse3 ? "SSE3," : "",
			hw_info.sse41 ? "SSE41," : "",
			hw_info.sse42 ? "SSE42," : "",
			hw_info.avx ? "AVX," : "",
			hw_info.avx2 ? "AVX2" : "");

		ImGui::Separator();

		ImGui::Text("GPU:");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(255, 255, 0, 255), "vendor %u device %u", hw_info.gpu_vendor, hw_info.gpu_device);

		ImGui::Text("Brand: ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(255, 255, 0, 255), "%s", hw_info.gpu_brand);

		ImGui::Text("VRAM Budget: ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(255, 255, 0, 255), "%.1f Mb", hw_info.vram_mb_budget);

		ImGui::Text("VRAM Usage: ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(255, 255, 0, 255), "%.1f Mb", hw_info.vram_mb_usage);

		ImGui::Text("VRAM Available: ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(255, 255, 0, 255), "%.1f Mb", hw_info.vram_mb_available);

		ImGui::Text("VRAM Reserved: ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(255, 255, 0, 255), "%.1f Mb", hw_info.vram_mb_reserved);
	}

	ImGui::End();
	return true;
}