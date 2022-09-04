#include <torn.h>
#include <assert.h>


i32 main(void)
{
    TORN_Init();
    
    /////////////////////////////////////////////////////////////////
    OS_App* app = OS_ConstructApp("Hello, Torn!", 
                                  OS_AppDefault,
                                  OS_AppDefault,
                                  OS_AppStyleDefault);
    assert(app && "Failed to create window");
    /////////////////////////////////////////////////////////////////
    
    GFX_Renderer* renderer = GFX_CreateRenderer(app);
    assert(renderer && "Failed to create renderer");
    
    u32 indices[] = {  // note that we start from 0!
        0, 1, 3,   // first triangle
        1, 2, 3    // second triangle
    };  
    GFX_ElementBuffer element_buffer = GFX_CreateElementBuffer(sizeof(indices),
                                                               indices);
    
    
    while (app->running)
    {
        OS_PollEvents(app);
        
        
        
        GFX_Clear(renderer);
        
        
        GFX_StartDraw();
        GFX_PushElementBuffer(element_buffer);
        GFX_PushV3(v3(0.5f,  0.5f, 0.0f));
        GFX_PushV3(v3( 0.5f, -0.5f, 0.0f));
        GFX_PushV3(v3( -0.5f, -0.5f, 0.0f));
        GFX_PushV3(v3( -0.5f,  0.5f, 0.0f  ));
        GFX_EndDrawIndexed(6, GFX_Triangles);
        
        
        GFX_Present(renderer);
    }
    
    TORN_Destroy();
    return 0;
}

