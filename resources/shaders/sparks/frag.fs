varying vec2 v_texCoord;
uniform sampler2D u_Texture;
uniform float u_Time;

void main()
{
    vec2 resolution = vec2(256, 256);
    //vec2 v_texCoord = 0.2 * (v_texCoord + v_texCoord - resolution) / resolution.y;    
    vec2 adjustedCoord = v_texCoord - vec2(0.5, 0.5);

    vec4 colorScale = vec4(1,2,3,0);
    gl_FragColor = texture2D(u_Texture, v_texCoord);

    for (float a = 0.5, i = 0.0;
         i < 20.0; 
         ++i, a += 0.03, ++u_Time)
    {
     // Affects number of particles on screen
     resolution = cos(u_Time - 7.0 * adjustedCoord * pow(a, i)) - 5.0 * adjustedCoord; 
     
     // Alters particle trajectory
     adjustedCoord *= mat2(cos(i + 0.02 * u_Time - vec4(0, 11, 33, 0)));

     // Creates tesselation / distortion effect
                      // Contributes to tesselation
     adjustedCoord += tanh(40.0 * dot(adjustedCoord, adjustedCoord) * cos(adjustedCoord.yx + u_Time)) 
                           
                      / 200.0

                      // Affects zoom on effect
                      + (0.2 * a * adjustedCoord)

                      // Creates dark bands
                      + cos(4.0 / exp(dot(gl_FragColor , gl_FragColor)/100.0) + u_Time)
                           
                      // Limits size of effect
                      / 300.0;
                           
     gl_FragColor += (1.0 + cos(colorScale + u_Time)) /
                     length(
                         (1.0 + i * dot(resolution,resolution)) *
                         sin(1.5 * adjustedCoord/(0.5 - dot(adjustedCoord,adjustedCoord)) -
                         9.0 * adjustedCoord.yx + u_Time)
                           );
    }

        
        
                    // Controls intensity of colors, high value creates fishbowl effect
     gl_FragColor = 25.0

                    / ( // Controls color, if not min then black appears in center of dots
                         min(gl_FragColor, 13.0) 

                         // Not-zero policy, also makes dots bright in center
                         + (240.0 / gl_FragColor)
                      )
                    
                    // Some weird position stuff
                    - dot(adjustedCoord, adjustedCoord)

                    // Affects size effect, smaller = smaller area
                    / 800.0;
}