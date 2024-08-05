// Math for Game Programmers: Fast and Funky 1D Nonlinear Transformations
//   https://www.youtube.com/watch?v=mr5xkf6zSzk

#ifndef INTERPOLATE_H
#define INTERPOLATE_H 

#define smooth_start_2( t ) ( t * t )
#define smooth_start_3( t ) ( smooth_start_2( t ) * t )
#define smooth_start_4( t ) ( smooth_start_3( t ) * t )
#define smooth_start_5( t ) ( smooth_start_4( t ) * t )
#define smooth_start_6( t ) ( smooth_start_5( t ) * t )

#define smooth_stop_2( t ) ( flip( smooth_start_2( flip( t ) ) ) )
#define smooth_stop_3( t ) ( flip( smooth_start_3( flip( t ) ) ) )
#define smooth_stop_4( t ) ( flip( smooth_start_4( flip( t ) ) ) )
#define smooth_stop_5( t ) ( flip( smooth_start_5( flip( t ) ) ) )
#define smooth_stop_6( t ) ( flip( smooth_start_6( flip( t ) ) ) )

#define smooth_step_3( t ) ( crossfade( smooth_start_2( t ), smooth_stop_2( t ), t ) ) 
#define mix( a, b, b_weight ) ( a + b_weight * ( b - a ) )
#define crossfade( a, b, t ) ( a + t * ( b - a ) )
#define scale( a, t ) ( t * a )
#define reverse_scale( a, t ) ( ( 1 - t ) * a )

#define arch_2( t ) ( scale( flip( t ), t ) )
#define smooth_start_arch_3( t ) ( scale( arch_2( t ), t ) )
#define smooth_stop_arch_3( t ) ( reverse_scale( arch_2( t ), t ) )

#define flip( x ) ( 1 - x )

#endif