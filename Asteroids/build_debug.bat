cl.exe /EHsc /GR- /DDEBUG /D_DEBUG /DUNICODE /DWIN32_LEAN_AND_MEAN /DWIN32 /D_CRT_SECURE_NO_WARNINGS /DVK_USE_PLATFORM_WIN32_KHR /IC:\VulkanSDK\1.2.131.2/Include /IC:\Users\nihal\Documents\Asteroids/Asteroids /IC:\Users\nihal\Documents\Asteroids/External/glm /IC:\Users\nihal\Documents\Asteroids/External/tinygltf /MDd /W3 /Z7 /MP Asteroids/main.cpp Asteroids/game.cpp Asteroids/common_graphics.cpp Asteroids/scene.cpp Asteroids/scene_graphics.cpp Asteroids/utils.cpp Asteroids/test_scene.cpp Asteroids/vk_utils.cpp Asteroids/image.cpp Asteroids/material.cpp Asteroids/scene_physics.cpp Asteroids/gltf_models.cpp Asteroids/static_mesh.cpp Asteroids/static_graphics_primitive.cpp Asteroids/static_graphics_primitives.cpp Asteroids/scene_assets.cpp Asteroids/actor.cpp Shlwapi.lib kernel32.lib user32.lib gdi32.lib winspool.lib shell32.lib ole32.lib oleaut32.lib uuid.lib comdlg32.lib advapi32.lib vulkan-1.lib /FoC:\Users\nihal\Documents\Asteroids/build32/vscode/debug/ /FeC:\Users\nihal\Documents\Asteroids/build32/vscode/debug/Asteroids.exe /link /LIBPATH:C:\VulkanSDK\1.2.131.2/Lib32 /DEBUG:FASTLINK /MACHINE:X86 /SUBSYSTEM:WINDOWS /incremental