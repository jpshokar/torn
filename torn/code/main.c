#include <torn.h>
#include <assert.h>


i32 main(void)
{
    TORN_Init();
    
    /////////////////////////////////////////////////////////////////
    OS_App* app = OS_ConstructApp("Demo", 
                                  OS_AppDefault,
                                  OS_AppDefault,
                                  OS_AppStyleDefault);
    assert(app && "Failed to create window");
    /////////////////////////////////////////////////////////////////
    //OS_SetWindowBorderlessFullscreen(app);
    GFX_Renderer* renderer = GFX_CreateRenderer(app);
    assert(renderer && "Failed to create renderer");
    
    GFX_Rect background = GFX_CreateRect(v2(app->size.x, app->size.y), v2(0,0));
    GFX_SetRectColor(&background, v4(10,10,10,255));
    
    V2F platform_pos = v2(0, (app->size.y/2)+100);
    V2F platform_size = v2(app->size.x,    100);
    
    GFX_Rect platform = GFX_CreateRect(platform_size,
                                       platform_pos
                                       );
    
    /*{
        platform.color.top_left  =    v4(89,89,89,255);
        platform.color.top_right =    v4(89,89,89,255);
        platform.color.bottom_left  = v4(3,1,10,255);
        platform.color.bottom_right  = v4(3,1,10,255);
    }
    */
    
    GFX_Rect cursor = GFX_CreateRect(v2(50,50), v2(50,50));
    GFX_SetRectColor(&platform, v4(20,20,20,255));
    
    GFX_Rect player = GFX_CreateRect(v2(100,170), v2(250,0));
    {
        //GFX_SetRectColor(&player, 
        GFX_Texture2D texture = GFX_LoadTexture2D("player.png");
        player.texture = texture;
    }
    
    V2F player_velocity = v2(0,0); 
    i32 intenisty = 8;
    GFX_PushShader(renderer->default_shader);
    GFX_GLSLShader1i(&renderer->default_shader, "intenisty", intenisty);
    
    GFX_Font font = GFX_CreateFont("Roboto-Bold.ttf", 80);
    
    
    GFX_Text text  = GFX_CreateText("The Engine Demonstration Room.\nClick anywhere to move the player.", v2(100,255), font);
    
    GFX_Framebuffer framebuffer = GFX_CreateFramebuffer(renderer, GFX_Format_RGB);
    i32 reflection_size = 1;
    GFX_Rect reflection = GFX_CreateRect(v2(app->size.w,
                                            (app->size.h - platform.pos.y) * reflection_size
                                            ), v2(0,platform.pos.y+platform.size.y));
    reflection.texture = framebuffer.texture;
    GFX_SetRectColor(&reflection, v4(100,100,100,255));
    
    GFX_Rect full = GFX_CreateRect(v2(app->size.w, app->size.h), v2(0,0));
    full.texture = framebuffer.texture;
    GFX_FlipRectUV(&full);
    
    
    // TODO (demo): 
    
    // SEPERATE SHADER AND/OR FRAMEBUFFER FOR THE POST PROCESSING EFFECTS??
    u8* vertex_shader =
        "#version 330\n"
        "layout (location = 0) in vec3 i_pos;\n"
        "layout (location = 1) in vec4 i_color;\n"
        "layout (location = 2) in vec2 i_uv;\n"
        "layout (location = 3) in vec4 i_normals;\n"
        "uniform mat4 orthographic;\n"
        "uniform mat4 projection;\n"
        "uniform mat4 model;\n"
        "uniform mat4 view;\n"
        "out vec4 v_color;\n"
        "out vec2 v_uv;\n"
        "out vec2 v_pos;\n"
        "out mat4 mvp;\n"
        "void main()\n"
        "{\n"
        "\tmvp = orthographic * model * view;\n"
        "\tv_uv =i_uv;\n"
        "\tv_pos =    (orthographic * model * view * vec4(i_pos, 1)).xy;\n"
        "\tv_color =  i_color;\n"
        "\tgl_Position = orthographic * model * view * vec4(i_pos, 1);\n"
        "}\n\0";
    
    u8* fragment_shader = 
        "#version 330\n"
        "out vec4 f_color;\n"
        "in vec4 v_color;\n"
        "in vec2 v_uv;\n"
        "in vec2 v_pos;\n"
        "in mat4 mvp;\n"
        
        "uniform sampler2D t_texture2d;\n"
        "uniform int intenisty;\n"
        "void main() {\n"
        "vec2 light_pos = vec4(vec4(0,0,0,0) * mvp).xy;\n"
        "vec4 ambient = vec4(0,0,0, 1);\n"
        "vec2 light = (mvp * vec4(light_pos,0,1)).xy;\n"
        
        "vec2 light_2_frag = light_pos - v_pos;\n"
        "light_2_frag.y = light_2_frag.y / intenisty;\n"
        "light_2_frag.x = light_2_frag.x / intenisty;\n"
        "float vec_length = clamp(length(light_2_frag) * 2 , 0, 1);\n"
        // 
        "\tf_color = texture(t_texture2d, v_uv) * v_color * (clamp(ambient + vec4(1-vec_length,1-vec_length, 1-vec_length, 1), 0, 1));"
        "}\n\0";
    GFX_Shader shader = GFX_CreateShader(vertex_shader, fragment_shader);
    
    while (app->running)
    {
        OS_PollEvents(app);
        
#if 0
        platform.size.x = platform_size.x;
        platform.size.y = platform_size.y;
        platform.pos.x = platform_pos.x;
        platform.pos.y = platform_pos.y;
        platform_pos.y = OS_GetMousePosition(app).y;
#endif
        
        if (app->event.type == OS_EventLMousePressed){
            player_velocity.y = 5;
            player.pos.x = OS_GetMousePosition(app).x - (player.size.w /2);
            player.pos.y = OS_GetMousePosition(app).y - (player.size.h /2);
        }
        
        reflection.size.w = app->size.w;
        reflection.size.h = ((app->size.h - platform.pos.y)) * reflection_size;
        
        refl B ection.pos.x = 0;
        reflection.pos.y = (platform.pos.y+platform.size.y);
        
#if 0
        if ( (player.pos.x - player.size.w) >= (platform.size.w - player.size.w)  )
        {
            
            player.pos.y = -20;
            player.pos.x = -player.size.w;
            player_velocity.x = 5;
        }  else {
            
            player_velocity.x = -5;
        }
#endif
        player_velocity.y -= 1.5; /* gravity */
        player.pos.y -= player_velocity.y;
        player.pos.x -= player_velocity.x;
        player_velocity.y *= 0.9; /* friction */
        player_velocity.x *= 0.9; /* friction */
        
        
        
        
        if ( !((player.pos.y + player.size.h) < platform_pos.y))
        {
            player_velocity.y = 0; 
            player.pos.y      = platform_pos.y - player.size.h;
        }
        
        
        GFX_PushFramebuffer(framebuffer); {
            
            GFX_Clear(renderer);
            GFX_DrawRect(&background, renderer);
            GFX_DrawText(&text, renderer);
            
            GFX_DrawRect(&platform, renderer);
            GFX_DrawRect(&player, renderer);
            
        } GFX_PopFramebuffer();
        
        
        GFX_Clear(renderer);
        GFX_DrawRect(&full, renderer);
        
        
        
        
        GFX_Present(renderer);
    }
    
    TORN_Destroy();
    return 0;
}

