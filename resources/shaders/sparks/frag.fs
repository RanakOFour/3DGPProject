uniform float u_Time;

void main()
{
    vec2 resolution = vec2(1000, 800);
    vec2 uvRange = .2*(gl_FragCoord.xy + gl_FragCoord.xy - resolution) / resolution.y;    
         
    vec4 colorScale = vec4(1,2,3,0);
    gl_FragColor = colorScale;

    for (float a = 0.5, t = u_Time, i; 
         ++i < 19.;
         gl_FragColor += (1.0 + cos(colorScale + t)) /
                         length((1.0 + i * dot(resolution,resolution)) *
                                sin(1.5 * uvRange/(0.5 - dot(uvRange,uvRange)) -
                                9.0 * uvRange.yx + t)
                               )
        )

        
        resolution = cos(++t - 7.0 * uvRange * pow(a += 0.03, i)) - 5.0 * uvRange, 
        
        uvRange += tanh(40. * dot(uvRange *= mat2(cos(i + .02*t - vec4(0,11,33,0)))
                                  ,uvRange)
                        * cos(1e2*uvRange.yx + t)) / 2e2
                   + .2 * a * uvRange
                   + cos(4./exp(dot(gl_FragColor,gl_FragColor)/1e2) + t) / 3e2;
              
     gl_FragColor = 25.6 / (min(gl_FragColor, 13.) + 164. / gl_FragColor) 
       - dot(uvRange, uvRange) / 250.;
}