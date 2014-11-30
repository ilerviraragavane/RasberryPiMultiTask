#include <math.h>

// array of 256 sin values
float sindata[] = {
    0.0, 0.0245412285229, 0.0490676743274, 0.0735645635997, 0.0980171403296, 0.122410675199, 0.146730474455, 0.17096188876, 0.195090322016,
    0.219101240157, 0.242980179903, 0.266712757475, 0.290284677254, 0.313681740399, 0.336889853392, 0.359895036535, 0.382683432365, 0.405241314005,
    0.42755509343, 0.449611329655, 0.471396736826, 0.49289819223, 0.514102744193, 0.534997619887, 0.55557023302, 0.575808191418, 0.595699304492,
    0.615231590581, 0.634393284164, 0.653172842954, 0.671558954847, 0.689540544737, 0.707106781187, 0.724247082951, 0.740951125355, 0.757208846506,
    0.773010453363, 0.788346427627, 0.803207531481, 0.817584813152, 0.831469612303, 0.84485356525, 0.85772861, 0.870086991109, 0.881921264348,
    0.893224301196, 0.903989293123, 0.914209755704, 0.923879532511, 0.932992798835, 0.941544065183, 0.949528180593, 0.956940335732, 0.963776065795,
    0.970031253195, 0.975702130039, 0.980785280403, 0.985277642389, 0.989176509965, 0.992479534599, 0.995184726672, 0.997290456679, 0.998795456205,
    0.999698818696, 1.0, 0.999698818696, 0.998795456205, 0.997290456679, 0.995184726672, 0.992479534599, 0.989176509965, 0.985277642389, 0.980785280403,
    0.975702130039, 0.970031253195, 0.963776065795, 0.956940335732, 0.949528180593, 0.941544065183, 0.932992798835, 0.923879532511, 0.914209755704,
    0.903989293123, 0.893224301196, 0.881921264348, 0.870086991109, 0.85772861, 0.84485356525, 0.831469612303, 0.817584813152, 0.803207531481,
    0.788346427627, 0.773010453363, 0.757208846506, 0.740951125355, 0.724247082951, 0.707106781187, 0.689540544737, 0.671558954847, 0.653172842954,
    0.634393284164, 0.615231590581, 0.595699304492, 0.575808191418, 0.55557023302, 0.534997619887, 0.514102744193, 0.49289819223, 0.471396736826,
    0.449611329655, 0.42755509343, 0.405241314005, 0.382683432365, 0.359895036535, 0.336889853392, 0.313681740399, 0.290284677254, 0.266712757475,
    0.242980179903, 0.219101240157, 0.195090322016, 0.17096188876, 0.146730474455, 0.122410675199, 0.0980171403296, 0.0735645635997, 0.0490676743274,
    0.0245412285229, 1.22464679915e-16, -0.0245412285229, -0.0490676743274, -0.0735645635997, -0.0980171403296, -0.122410675199, -0.146730474455,
    -0.17096188876, -0.195090322016, -0.219101240157, -0.242980179903, -0.266712757475, -0.290284677254, -0.313681740399, -0.336889853392,
    -0.359895036535, -0.382683432365, -0.405241314005, -0.42755509343, -0.449611329655, -0.471396736826, -0.49289819223, -0.514102744193,
    -0.534997619887, -0.55557023302, -0.575808191418, -0.595699304492, -0.615231590581, -0.634393284164, -0.653172842954, -0.671558954847,
    -0.689540544737, -0.707106781187, -0.724247082951, -0.740951125355, -0.757208846506, -0.773010453363, -0.788346427627, -0.803207531481,
    -0.817584813152, -0.831469612303, -0.84485356525, -0.85772861, -0.870086991109, -0.881921264348, -0.893224301196, -0.903989293123, -0.914209755704,
    -0.923879532511, -0.932992798835, -0.941544065183, -0.949528180593, -0.956940335732, -0.963776065795, -0.970031253195, -0.975702130039,
    -0.980785280403, -0.985277642389, -0.989176509965, -0.992479534599, -0.995184726672, -0.997290456679, -0.998795456205, -0.999698818696, -1.0,
    -0.999698818696, -0.998795456205, -0.997290456679, -0.995184726672, -0.992479534599, -0.989176509965, -0.985277642389, -0.980785280403,
    -0.975702130039, -0.970031253195, -0.963776065795, -0.956940335732, -0.949528180593, -0.941544065183, -0.932992798835, -0.923879532511,
    -0.914209755704, -0.903989293123, -0.893224301196, -0.881921264348, -0.870086991109, -0.85772861, -0.84485356525, -0.831469612303, -0.817584813152,
    -0.803207531481, -0.788346427627, -0.773010453363, -0.757208846506, -0.740951125355, -0.724247082951, -0.707106781187, -0.689540544737,
    -0.671558954847, -0.653172842954, -0.634393284164, -0.615231590581, -0.595699304492, -0.575808191418, -0.55557023302, -0.534997619887,
    -0.514102744193, -0.49289819223, -0.471396736826, -0.449611329655, -0.42755509343, -0.405241314005, -0.382683432365, -0.359895036535,
    -0.336889853392, -0.313681740399, -0.290284677254, -0.266712757475, -0.242980179903, -0.219101240157, -0.195090322016, -0.17096188876,
    -0.146730474455, -0.122410675199, -0.0980171403296, -0.0735645635997, -0.0490676743274, -0.0245412285229 }; 

float sinf(float x) {
    unsigned int index = 256.0 * x / (2 * PI);
    return sindata[index & 0xff];
}

