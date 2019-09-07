#include "Application.h"
#include "ModuleGUI.h"
#include "glew-2.1.0/include/GL/glew.h"

#include "SDL/include/SDL_opengl.h"

#include "imgui\imgui.h"
#include "imgui\imgui_impl_opengl2.h"
#include "imgui\imgui_impl_sdl.h"

#include "mmgr/mmgr.h"

ModuleGUI::ModuleGUI(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	name = "ModuleGUI";
}

bool ModuleGUI::Start()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	ImGui::StyleColorsDark();

	ImGui_ImplSDL2_InitForOpenGL(App->window->GetWindow(), App->renderer3D->GetContext());
	ImGui_ImplOpenGL2_Init();

	SDL_GetVersion(&sdl_version);

	return true;
}

update_status ModuleGUI::Update(float dt)
{
	ImGui_ImplOpenGL2_NewFrame();
	ImGui_ImplSDL2_NewFrame(App->window->GetWindow());
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

bool ModuleGUI::CleanUp()
{
	ImGui_ImplOpenGL2_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
	return true;
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
	ImGui::Text("External tools & libraries");

	// TODO: repassar les versions de les llibreries (no ficar strings directament)
	// TODO: passar tot el text a angles...

	if (ImGui::CollapsingHeader("Libraries"))
	{
		if (ImGui::Button("SDL"))
		{
			ShellExecuteA(NULL, "open", "https://www.libsdl.org/", NULL, NULL, SW_SHOWNORMAL);
		}
		ImGui::SameLine();
		ImGui::Text(" version: ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(255, 255, 0, 255), "%i.%i.%i", sdl_version.major, sdl_version.minor, sdl_version.patch);

		if (ImGui::Button("OpenGL"))
		{
			ShellExecuteA(NULL, "open", "https://www.opengl.org/", NULL, NULL, SW_SHOWNORMAL);
		}
		ImGui::SameLine();
		ImGui::Text(" version: ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(255, 255, 0, 255), "%s", glGetString(GL_VERSION));

		if (ImGui::Button("ImGui"))
		{
			ShellExecuteA(NULL, "open", "https://github.com/ocornut/imgui", NULL, NULL, SW_SHOWNORMAL);
		}
		ImGui::SameLine();
		ImGui::Text(" version: ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(255, 255, 0, 255), "%s", ImGui::GetVersion());

		if (ImGui::Button("GLEW"))
		{
			ShellExecuteA(NULL, "open", "https://github.com/nigels-com/glew", NULL, NULL, SW_SHOWNORMAL);
		}
		ImGui::SameLine();
		ImGui::Text(" version: ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(255, 255, 0, 255), "%i.%i.%i%i.", GLEW_VERSION, GLEW_VERSION_MAJOR, GLEW_VERSION_MINOR, GLEW_VERSION_MICRO);

		if (ImGui::Button("MathGeoLib"))
		{
			ShellExecuteA(NULL, "open", "https://github.com/juj/MathGeoLib", NULL, NULL, SW_SHOWNORMAL);
		}
		ImGui::SameLine();
		ImGui::Text(" version: ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(255, 255, 0, 255), "%s", "1.5");

		if (ImGui::Button("PCG"))
		{
			ShellExecuteA(NULL, "open", "http://www.pcg-random.org/", NULL, NULL, SW_SHOWNORMAL);
		}
		ImGui::SameLine();
		ImGui::Text(" version: ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(255, 255, 0, 255), "%s", "0.94");

		if (ImGui::Button("MMGR"))
		{
			ShellExecuteA(NULL, "open", "http://www.flipcode.com/archives/Presenting_A_Memory_Manager.shtml", NULL, NULL, SW_SHOWNORMAL);
		}

		if (ImGui::Button("Parson"))
		{
			ShellExecuteA(NULL, "open", "https://github.com/kgabis/parson", NULL, NULL, SW_SHOWNORMAL);
		}

	}

	if (ImGui::CollapsingHeader("Licenses"))
	{
		ImGui::Text("License: "); ImGui::SameLine();
		if (ImGui::Button("MIT License")) { ShellExecuteA(NULL, "open", "https://opensource.org/licenses/MIT", NULL, NULL, SW_SHOWNORMAL); }
	}

	ImGui::End();
	return true;
}

bool ModuleGUI::GUI_ConfigurationWindow()
{
	sMStats stats = m_getMemoryStatistics();
	static int speed = 0;
	static std::vector<float> memory(100);
	static std::vector<float> memoryFPS(100);
	static std::vector<float> memoryMS(100);
	if (++speed > 20)
	{
		speed = 0;
		if (memory.size() == 100)
		{
			for (uint i = 0; i < 100 - 1; ++i)
				memory[i] = memory[i + 1];

			memory[100 - 1] = (float)stats.totalReportedMemory;
		}
		else
			memory.push_back((float)stats.totalReportedMemory);
		if (memoryFPS.size() == 100)
		{
			for (uint i = 0; i < 100 - 1; ++i)
				memoryFPS[i] = memoryFPS[i + 1];

			memoryFPS[100 - 1] = App->GetLastFPS();
		}
		else
			memoryFPS.push_back(App->GetLastFPS());
		if (memoryMS.size() == 100)
		{
			for (uint i = 0; i < 100 - 1; ++i)
				memoryMS[i] = memoryMS[i + 1];

			memoryMS[100 - 1] = App->GetLastMS();
		}
		else
			memoryMS.push_back(App->GetLastMS());
	}

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
		if (ImGui::InputText("App Name", gui_appname, 256, ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll))
		{
			App->window->SetTitle(gui_appname);
		}

		static char gui_organization_name[256];
		strcpy_s(gui_organization_name, 256, App->window->organization);
		if (ImGui::InputText("Organization Name", gui_organization_name, 256, ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll))
		{
			App->window->organization = gui_organization_name;
		}

		static int tmp_max_fps = App->GetMaxFPS();
		if (ImGui::SliderInt("Max. fps", &tmp_max_fps, 0.0f, 120.0f))
		{
			App->SetMaxFPS(tmp_max_fps);
		}

		ImGui::PlotHistogram("##memory", &memory[0], memory.size(), 0, "Memory Consumption", 0.0f, (float)stats.peakReportedMemory * 1.2f, ImVec2(310, 100));
		ImGui::PlotHistogram("##fps", &memoryFPS[0], memoryFPS.size(), 0, "FPS", 0.0f, 100.0f, ImVec2(310, 100));
		ImGui::PlotHistogram("##milliseconds", &memoryMS[0], memoryMS.size(), 0, "Last Frame MS", 0.0f, 50.0f, ImVec2(310, 100));

		ImGui::Text("Total Reported Mem: %u", stats.totalReportedMemory);
		ImGui::Text("Total Actual Mem: %u", stats.totalActualMemory);
		ImGui::Text("Peak Reported Mem: %u", stats.peakReportedMemory);
		ImGui::Text("Peak Actual Mem: %u", stats.peakActualMemory);
		ImGui::Text("Accumulated Reported Mem: %u", stats.accumulatedReportedMemory);
		ImGui::Text("Accumulated Actual Mem: %u", stats.accumulatedActualMemory);
		ImGui::Text("Accumulated Alloc Unit Count: %u", stats.accumulatedAllocUnitCount);
		ImGui::Text("Total Alloc Unit Count: %u", stats.totalAllocUnitCount);
		ImGui::Text("Peak Alloc Unit Count: %u", stats.peakAllocUnitCount);
	}

	if (ImGui::CollapsingHeader("Window"))
	{
		static float winBrightness = 1.0f;
		if (ImGui::SliderFloat("Brightness", &winBrightness, 0, 1))
		{
			App->window->SetScreenBrightness(winBrightness);
		}
		// MIN RESOLUTION 640x480 
		if (ImGui::SliderInt("Window Width", &App->window->screenWidth, 640, App->window->maxScreenWidth))
		{
			App->window->SetWindowsSize();
		}
		if (ImGui::SliderInt("Window Height", &App->window->screenHeight, 480, App->window->maxScreenHeight))
		{
			App->window->SetWindowsSize();
		}
		
		ImGui::Text("Refresh Rate: ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(255, 255, 0, 255), "%i", App->window->refreshRate);

		if (ImGui::Checkbox("Full Screen", &App->window->winFullScreen))
		{
			App->window->SetFullScreen(App->window->winFullScreen);
		}
		if (ImGui::Checkbox("Resizable", &App->window->winResizable))
		{
			App->window->SetResizable(App->window->winResizable);
		}
		if (ImGui::Checkbox("Borderless", &App->window->winBorderless))
		{
			App->window->SetBorderless(App->window->winBorderless);
		}
		if (ImGui::Checkbox("Full Desktop", &App->window->winFullScreenDesktop))
		{
			App->window->SetFullScreenDesktop(App->window->winFullScreenDesktop);
		}
	}

	if (ImGui::CollapsingHeader("File System"))
	{
		// (TODO)
		ImGui::Text("Base Path: ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(255, 255, 0, 255), "%s");
	}

	if (ImGui::CollapsingHeader("Input"))
	{
		ImGui::SliderFloat("Camera Speed", &App->camera->camera_mov_speed, 1.0f, 10.0f);
		ImGui::SliderFloat("Mouse Wheel Speed", &App->camera->mouse_wheel_speed, 0.1f, 5.0f);
		ImGui::SliderFloat("Mouse Sensitivity", &App->camera->mouse_sensitivity, 0.01f, 2.0f);

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