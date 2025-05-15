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

    for (float a = 0.5, t = u_Time / 4., i; 
         ++i < 19;
         gl_FragColor += (1.0 + cos(colorScale + t)) /
                         length((1.0 + i * dot(resolution,resolution)) *
                                sin(1.5 * adjustedCoord/(0.5 - dot(adjustedCoord,adjustedCoord)) -
                                9.0 * adjustedCoord.yx + t)
                               )
        )

        
        resolution = cos(++t - 7.0 * adjustedCoord * pow(a += 0.03, i)) - 5.0 * adjustedCoord, 
        
        adjustedCoord += tanh(40. * dot(adjustedCoord *= mat2(cos(i + .02*t - vec4(0,11,33,0)))
                                  ,adjustedCoord)
                        * cos(1e2*adjustedCoord.yx + t)) / 2e2
                   + .2 * a * adjustedCoord
                   + cos(4./exp(dot(gl_FragColor,gl_FragColor)/1e2) + t) / 3e2;
              
     gl_FragColor = 25.6 / (min(gl_FragColor, 13.) + 164. / gl_FragColor) 
       - dot(adjustedCoord, adjustedCoord) / 250.;
}