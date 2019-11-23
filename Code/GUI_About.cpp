#include "Application.h"
#include "GUI_About.h"

#include "imgui\imgui.h"
#include "imgui\imgui_impl_opengl2.h"
#include "imgui\imgui_impl_sdl.h"

#include "Assimp/include/version.h"
#include "DevIL/include/IL/il.h"
#include "physfs/include/physfs.h"

GUI_About::GUI_About(SDL_Scancode shortcut) : GUI_Panel(shortcut, GUI_WINDOWS::GUI_ABOUT)
{}

void GUI_About::Draw()
{
	ImGui::Begin("About", &active, ImGuiWindowFlags_NoFocusOnAppearing);
	ImGui::SetWindowPos(ImVec2(App->window->screenWidth *.35f, App->window->screenHeight - App->window->screenHeight*.75f), ImGuiCond_Once);
	ImGui::SetWindowSize(ImVec2(App->window->screenWidth*0.3f, App->window->screenHeight*.15), ImGuiCond_Once);
	ImGui::Text("Socialist Engine is a video game engine made by students\nfrom CITM(UPC) at VideoGame Development & Design degree");
	ImGui::Text("Made by Isaac Calvis & Aitor Velez");

	ImGui::Text("License: MIT (More info at License file or at ReadMe)");
	ImGui::Text("External tools & libraries");

	if (ImGui::CollapsingHeader("Libraries"))
	{
		if (ImGui::Button("SDL"))
		{
			ShellExecuteA(NULL, "open", "https://www.libsdl.org/", NULL, NULL, SW_SHOWNORMAL);
		}
		ImGui::SameLine();
		ImGui::Text(" version: ");
		ImGui::SameLine();
		// TODO: FIX THAT
		//ImGui::TextColored(ImVec4(255, 255, 0, 255), "%i.%i.%i", sdl_version.major, sdl_version.minor, sdl_version.patch);

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

		if (ImGui::Button("Assimp"))
		{
			ShellExecuteA(NULL, "open", "http://cms.assimp.org/index.php", NULL, NULL, SW_SHOWNORMAL);
		}
		ImGui::SameLine();
		ImGui::Text(" version: ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(255, 255, 0, 255), "%i.%i.%i", aiGetVersionMajor(), aiGetVersionMinor(), aiGetVersionRevision());

		if (ImGui::Button("DevIl"))
		{
			ShellExecuteA(NULL, "open", "http://openil.sourceforge.net/", NULL, NULL, SW_SHOWNORMAL);
		}
		ImGui::SameLine();
		ImGui::Text(" version: ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(255, 255, 0, 255), "%i", ilGetInteger(IL_VERSION_NUM));

		if (ImGui::Button("MMGR"))
		{
			ShellExecuteA(NULL, "open", "http://www.flipcode.com/archives/Presenting_A_Memory_Manager.shtml", NULL, NULL, SW_SHOWNORMAL);
		}

		if (ImGui::Button("Parson"))
		{
			ShellExecuteA(NULL, "open", "https://github.com/kgabis/parson", NULL, NULL, SW_SHOWNORMAL);
		}

		if (ImGui::Button("Par Shapes"))
		{
			ShellExecuteA(NULL, "open", "https://github.com/prideout/par/blob/master/par_shapes.h", NULL, NULL, SW_SHOWNORMAL);
		}
		static PHYSFS_Version physfs_version;
		PHYSFS_getLinkedVersion(&physfs_version);
		if (ImGui::Button("PhysFS"))
		{
			ShellExecuteA(NULL, "open", "https://icculus.org/physfs/docs/html/index.html", NULL, NULL, SW_SHOWNORMAL);
		}
		ImGui::SameLine();
		ImGui::Text(" version: ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(255, 255, 0, 255), "%i.%i.%i", physfs_version.major, physfs_version.minor, physfs_version.patch);

		if (ImGui::Button("ImGuizmo"))
		{
			ShellExecuteA(NULL, "open", "https://github.com/CedricGuillemet/ImGuizmo", NULL, NULL, SW_SHOWNORMAL);
		}
	}

	if (ImGui::CollapsingHeader("Licenses"))
	{
		ImGui::Text("License: "); ImGui::SameLine();
		if (ImGui::Button("MIT License")) { ShellExecuteA(NULL, "open", "https://opensource.org/licenses/MIT", NULL, NULL, SW_SHOWNORMAL); }
	}

	ImGui::End();
}