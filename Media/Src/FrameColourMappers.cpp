/////////////////////////////////////////
// Frame.cpp
// Copyright (c) 2022 TXPCo Ltd
/////////////////////////////////////////

#include "pch.h"

#include <cassert>
#include <algorithm>
#include <string>

#include "../../Include/MediaFrame.h"

static int linearToSrgb8bit(float x);
float srgb8bitToLinear(unsigned char x);

namespace Media {

    ///////////////////////////////////////////////////////////////////////////////
    // FrameColourOutputMapper
    ///////////////////////////////////////////////////////////////////////////////

    FrameColourOutputMapper::FrameColourOutputMapper() {
    }

    FrameColourOutputMapper::~FrameColourOutputMapper() {
    }


    //////////////////////////////////////////////////////////////////////////////
    // FrameSrgbOutputMapper
    /////////////////////////////////////////////////////////////////////////////

    FrameSrgbOutputMapper::FrameSrgbOutputMapper() {
    }

    FrameSrgbOutputMapper::~FrameSrgbOutputMapper() {
    }

    void FrameSrgbOutputMapper::convert(FrameRgba8& output, const FrameRgbaHalf& input) {

        if (output.width() != input.width() || input.height() != output.height())
            THROW_ENRICHED(std::invalid_argument, Host::ExceptionSource::kCRT, EINVAL, "Input and output frames are not the same size.");

        for (unsigned y = 0; y < input.height(); y++) {
            for (unsigned x = 0; x < input.width(); x++) {
                Media::RgbaHalf valueIn;
                Media::Rgba8 valueOut;

                half channelValue = 0.0;

                valueIn = input.pixelAt(x, y);

                valueOut.r = linearToSrgb8bit (valueIn.r);
                valueOut.g = linearToSrgb8bit(valueIn.g);
                valueOut.b = linearToSrgb8bit(valueIn.b);
                channelValue = (valueIn.a * 255.0f);
                valueOut.a = std::clamp(static_cast<int>(channelValue), 0, 255);

                output.setPixelAt(x, y, valueOut);
            }
        }

    }

    ///////////////////////////////////////////////////////////////////////////////
    // FrameColourInputMapper
    ///////////////////////////////////////////////////////////////////////////////

    FrameColourInputMapper::FrameColourInputMapper() {
    }

    FrameColourInputMapper::~FrameColourInputMapper() {
    }


    //////////////////////////////////////////////////////////////////////////////
    // FrameSrgbInputMapper
    /////////////////////////////////////////////////////////////////////////////

    FrameSrgbInputMapper::FrameSrgbInputMapper() {
    }

    FrameSrgbInputMapper::~FrameSrgbInputMapper() {
    }

    void FrameSrgbInputMapper::convert(FrameRgbaHalf& output, const FrameRgba8& input) {

        if (output.width() != input.width() || input.height() != output.height())
            THROW_ENRICHED(std::invalid_argument, Host::ExceptionSource::kCRT, EINVAL, "Input and output frames are not the same size.");

        for (unsigned y = 0; y < input.height(); y++) {
            for (unsigned x = 0; x < input.width(); x++) {
                Media::Rgba8 valueIn;
                Media::RgbaHalf valueOut;

                half channelValue = 0.0f;

                valueIn = input.pixelAt(x, y);

                valueOut.r = srgb8bitToLinear(valueIn.r);
                valueOut.g = srgb8bitToLinear(valueIn.g);
                valueOut.b = srgb8bitToLinear(valueIn.b);
                channelValue = (static_cast<float>(valueIn.a) / 255.0f);
                valueOut.a = std::clamp(static_cast<float>(channelValue), 0.0f, 1.0f);

                output.setPixelAt(x, y, valueOut);
            }
        }

    }
}

/////////////////////////////////////////////////////////////////////////////
// https://www.nayuki.io/page/srgb-transform-library
/////////////////////////////////////////////////////////////////////////////

const double SRGB_8BIT_TO_LINEAR_DOUBLE[1 << 8] = {
    0.0, 3.035269835488375e-4, 6.07053967097675e-4, 9.105809506465125e-4,
    0.00121410793419535, 0.0015176349177441874, 0.001821161901293025, 0.0021246888848418626,
    0.0024282158683907, 0.0027317428519395373, 0.003035269835488375, 0.003346535763899161,
    0.003676507324047436, 0.004024717018496307, 0.004391442037410293, 0.004776953480693729,
    0.005181516702338386, 0.005605391624202723, 0.006048833022857054, 0.006512090792594475,
    0.006995410187265387, 0.007499032043226175, 0.008023192985384994, 0.008568125618069307,
    0.009134058702220787, 0.00972121732023785, 0.010329823029626936, 0.010960094006488246,
    0.011612245179743885, 0.012286488356915872, 0.012983032342173012, 0.013702083047289686,
    0.014443843596092545, 0.01520851442291271, 0.01599629336550963, 0.016807375752887384,
    0.017641954488384078, 0.018500220128379697, 0.019382360956935723, 0.0202885630566524,
    0.021219010376003555, 0.022173884793387385, 0.02315336617811041, 0.024157632448504756,
    0.02518685962736163, 0.026241221894849898, 0.027320891639074894, 0.028426039504420793,
    0.0295568344378088, 0.030713443732993635, 0.03189603307301153, 0.033104766570885055,
    0.03433980680868217, 0.03560131487502034, 0.03688945040110004, 0.0382043715953465,
    0.03954623527673284, 0.04091519690685319, 0.042311410620809675, 0.043735029256973465,
    0.04518620438567554, 0.046665086336880095, 0.04817182422688942, 0.04970656598412723,
    0.05126945837404324, 0.052860647023180246, 0.05448027644244237, 0.05612849004960009,
    0.05780543019106723, 0.0595112381629812, 0.06124605423161761, 0.06301001765316767,
    0.06480326669290577, 0.06662593864377289, 0.06847816984440017, 0.07036009569659588,
    0.07227185068231748, 0.07421356838014963, 0.07618538148130785, 0.07818742180518633,
    0.08021982031446832, 0.0822827071298148, 0.08437621154414882, 0.08650046203654976,
    0.08865558628577294, 0.09084171118340768, 0.09305896284668745, 0.0953074666309647,
    0.09758734714186246, 0.09989872824711389, 0.10224173308810132, 0.10461648409110419,
    0.10702310297826761, 0.10946171077829933, 0.1119324278369056, 0.11443537382697373,
    0.11697066775851084, 0.11953842798834562, 0.12213877222960187, 0.12477181756095049,
    0.12743768043564743, 0.1301364766903643, 0.13286832155381798, 0.13563332965520566,
    0.13843161503245183, 0.14126329114027164, 0.14412847085805777, 0.14702726649759498,
    0.14995978981060856, 0.15292615199615017, 0.1559264637078274, 0.1589608350608804,
    0.162029375639111, 0.1651321945016676, 0.16826940018969075, 0.1714411007328226,
    0.17464740365558504, 0.17788841598362912, 0.18116424424986022, 0.184474994500441,
    0.18782077230067787, 0.19120168274079138, 0.1946178304415758, 0.19806931955994886,
    0.20155625379439707, 0.20507873639031693, 0.20863687014525575, 0.21223075741405523,
    0.21586050011389926, 0.2195261997292692, 0.2232279573168085, 0.22696587351009836,
    0.23074004852434915, 0.23455058216100522, 0.238397573812271, 0.24228112246555486,
    0.24620132670783548, 0.25015828472995344, 0.25415209433082675, 0.2581828529215958,
    0.26225065752969623, 0.26635560480286247, 0.2704977910130658, 0.27467731206038465,
    0.2788942634768104, 0.2831487404299921, 0.2874408377269175, 0.29177064981753587,
    0.2961382707983211, 0.3005437944157765, 0.3049873140698863, 0.30946892281750854,
    0.31398871337571754, 0.31854677812509186, 0.32314320911295075, 0.3277780980565422,
    0.33245153634617935, 0.33716361504833037, 0.3419144249086609, 0.3467040563550296,
    0.35153259950043936, 0.3564001441459435, 0.3613067797835095, 0.3662525955988395,
    0.3712376804741491, 0.3762621229909065, 0.38132601143253014, 0.386429433787049,
    0.39157247774972326, 0.39675523072562685, 0.4019777798321958, 0.4072402119017367,
    0.41254261348390375, 0.4178850708481375, 0.4232676699860717, 0.4286904966139066,
    0.43415363617474895, 0.4396571738409188, 0.44520119451622786, 0.45078578283822346,
    0.45641102318040466, 0.4620769996544071, 0.467783796112159, 0.47353149614800955,
    0.4793201831008268, 0.4851499400560704, 0.4910208498478356, 0.4969329950608704,
    0.5028864580325687, 0.5088813208549338, 0.5149176653765214, 0.5209955732043543,
    0.5271151257058131, 0.5332764040105052, 0.5394794890121072, 0.5457244613701866,
    0.5520114015120001, 0.5583403896342679, 0.5647115057049292, 0.5711248294648731,
    0.5775804404296506, 0.5840784178911641, 0.5906188409193369, 0.5972017883637634,
    0.6038273388553378, 0.6104955708078648, 0.6172065624196511, 0.6239603916750761,
    0.6307571363461468, 0.6375968739940326, 0.6444796819705821, 0.6514056374198242,
    0.6583748172794485, 0.665387298282272, 0.6724431569576875, 0.6795424696330938,
    0.6866853124353135, 0.6938717612919899, 0.7011018919329731, 0.7083757798916868,
    0.7156935005064807, 0.7230551289219693, 0.7304607400903537, 0.7379104087727308,
    0.7454042095403874, 0.7529422167760779, 0.7605245046752924, 0.768151147247507,
    0.7758222183174236, 0.7835377915261935, 0.7912979403326302, 0.799102738014409,
    0.8069522576692516, 0.8148465722161012, 0.8227857543962835, 0.8307698767746546,
    0.83879901174074, 0.846873231509858, 0.8549926081242338, 0.8631572134541023,
    0.8713671191987972, 0.8796223968878317, 0.8879231178819663, 0.8962693533742664,
    0.9046611743911496, 0.9130986517934192, 0.9215818562772946, 0.9301108583754237,
    0.938685728457888, 0.9473065367331999, 0.9559733532492861, 0.9646862478944651,
    0.9734452903984125, 0.9822505503331171, 0.9911020971138298, 1.0,
};

int linearToSrgb8bit(float x) {
    if (x <= 0.0)
        return 0;
    if (x >= 1.0)
        return 255;
    const double* TABLE = SRGB_8BIT_TO_LINEAR_DOUBLE;
    int y = 0;
    for (int i = 128; i != 0; i >>= 1) {
        if (TABLE[y + i] <= x)
            y += i;
    }
    if (x - TABLE[y] <= TABLE[y + 1] - x)
        return y;
    else
        return y + 1;
}

float srgb8bitToLinear (unsigned char x) {
    
    float xf = static_cast<float>(x) / 255.0f;

    if (xf < 0.04045f)
        return xf / 12.92f;
    else
        return std::pow((xf + 0.055f) / 1.055f, 2.4f);
}