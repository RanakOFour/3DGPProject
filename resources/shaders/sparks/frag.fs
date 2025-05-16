varying vec2 v_texCoord;
uniform sampler2D u_Texture;
uniform float u_Time;

void main()
{
    vec2 resolution = vec2(256, 256);
    
    // Used for swirling effects
    vec2 adjustedCoord = v_texCoord - vec2(0.5, 0.5);

    vec4 colorScale = vec4(1,2,3,0);
    gl_FragColor = texture2D(u_Texture, v_texCoord);

    for (float a = 0.5, i = 0.0, t = u_Time;
         i < 20.0; 
         ++i, a += 0.03, ++t)
    {
     // Affects number of particles on screen
     resolution = cos(t - 7.0 * adjustedCoord * pow(a, i)) - 5.0 * adjustedCoord; 
     
     // Rotates coordinates
     adjustedCoord *= mat2(cos(i + 0.02 * t - vec4(0, 11, 33, 0)));

     adjustedCoord += tanh(40.0 * dot(adjustedCoord, adjustedCoord) * cos(adjustedCoord.yx + t)) 
                           
                      / 200.0
                      + (0.2 * a * adjustedCoord)
                      + cos(4.0 / exp(dot(gl_FragColor , gl_FragColor) / 100.0) + t)
                      / 300.0;
                           

     gl_FragColor += (1.0 + cos(colorScale + t)) /
                     // Adjusting by length creates bright spots
                     length(
                         (1.0 + i * dot(resolution,resolution)) *
                         sin(1.5 * adjustedCoord/(0.5 - dot(adjustedCoord,adjustedCoord)) -
                         9.0 * adjustedCoord.yx + t)
                           );
    }

        
        
                    // Controls intensity of colors, high value creates fishbowl effect
     gl_FragColor = 25.0
                    / ( // Limits brightness
                         min(gl_FragColor, 13.0) 
                         + (240.0 / gl_FragColor)
                      )
                    // Darkens edges
                    - dot(adjustedCoord, adjustedCoord) / 800.0;
}


