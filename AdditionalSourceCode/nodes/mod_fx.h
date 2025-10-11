#pragma once

#include "granular.h"
// These will improve the readability of the connection definition

#define getT(Idx) template get<Idx>()
#define connectT(Idx, target) template connect<Idx>(target)
#define getParameterT(Idx) template getParameter<Idx>()
#define setParameterT(Idx, value) template setParameter<Idx>(value)
#define setParameterWT(Idx, value) template setWrapParameter<Idx>(value)
using namespace scriptnode;
using namespace snex;
using namespace snex::Types;

namespace mod_fx_impl
{
// ==============================| Node & Parameter type declarations |==============================

using pack2_writer_t = wrap::data<control::pack2_writer, 
                                  data::external::sliderpack<2>>;
template <int NV>
using smoothed_parameter2_t = wrap::mod<parameter::plain<pack2_writer_t, 0>, 
                                        control::smoothed_parameter<NV, smoothers::linear_ramp<NV>>>;

template <int NV>
using smoothed_parameter_t = wrap::mod<parameter::plain<pack2_writer_t, 1>, 
                                       control::smoothed_parameter<NV, smoothers::linear_ramp<NV>>>;

DECLARE_PARAMETER_RANGE(pma_modRange, 
                        0., 
                        100.);

template <int NV>
using pma_mod = parameter::from0To1<project::granular<NV>, 
                                    2, 
                                    pma_modRange>;

template <int NV>
using pma_t = control::pma<NV, pma_mod<NV>>;
template <int NV>
using smoothed_parameter1_t = wrap::mod<parameter::plain<pma_t<NV>, 0>, 
                                        control::smoothed_parameter<NV, smoothers::linear_ramp<NV>>>;

template <int NV>
using cable_pack_mod = parameter::chain<ranges::Identity, 
                                        parameter::plain<smoothed_parameter_t<NV>, 0>, 
                                        parameter::plain<smoothed_parameter1_t<NV>, 0>>;

template <int NV>
using cable_pack_t = wrap::data<control::cable_pack<cable_pack_mod<NV>>, 
                                data::external::sliderpack<0>>;

template <int NV>
using clock_ramp_mod = parameter::chain<ranges::Identity, 
                                        parameter::plain<cable_pack_t<NV>, 0>, 
                                        parameter::plain<smoothed_parameter2_t<NV>, 0>>;

template <int NV>
using clock_ramp_t = wrap::mod<clock_ramp_mod<NV>, 
                               wrap::no_data<core::clock_ramp<NV, false>>>;

template <int NV>
using modchain_t_ = container::chain<parameter::empty, 
                                     wrap::fix<1, clock_ramp_t<NV>>>;

template <int NV>
using modchain_t = wrap::control_rate<modchain_t_<NV>>;

template <int NV>
using mods_t = container::chain<parameter::empty, 
                                wrap::fix<2, modchain_t<NV>>, 
                                cable_pack_t<NV>, 
                                smoothed_parameter_t<NV>>;

template <int NV>
using tempo_sync_t = wrap::mod<parameter::plain<project::granular<NV>, 6>, 
                               control::tempo_sync<NV>>;

DECLARE_PARAMETER_RANGE_SKEW(dry_wet_mixer2_c0Range, 
                             -100., 
                             0., 
                             5.42227);

template <int NV>
using dry_wet_mixer2_c0 = parameter::from0To1<core::gain<NV>, 
                                              0, 
                                              dry_wet_mixer2_c0Range>;

DECLARE_PARAMETER_RANGE_SKEW(cable_table_modRange, 
                             0., 
                             6., 
                             5.42227);

template <int NV>
using cable_table_mod = parameter::from0To1<core::gain<NV>, 
                                            0, 
                                            cable_table_modRange>;

struct cable_table_t_data
{
	span<float, 512> data =
	{
		0.f, 0.00406539f, 0.00813073f, 0.0121961f, 0.0162615f, 0.0203269f,
		0.0243922f, 0.0284576f, 0.0325229f, 0.0365883f, 0.0406537f, 0.044719f,
		0.0487844f, 0.0528498f, 0.0569152f, 0.0609805f, 0.0650459f, 0.0691113f,
		0.0731766f, 0.077242f, 0.0813074f, 0.0853727f, 0.0894381f, 0.0935035f,
		0.0975689f, 0.101634f, 0.1057f, 0.109765f, 0.11383f, 0.117896f,
		0.121961f, 0.126026f, 0.130092f, 0.134157f, 0.138223f, 0.142288f,
		0.146353f, 0.150419f, 0.154484f, 0.158549f, 0.162615f, 0.16668f,
		0.170745f, 0.174811f, 0.178876f, 0.182942f, 0.187007f, 0.191072f,
		0.195138f, 0.199203f, 0.203268f, 0.207334f, 0.211399f, 0.215465f,
		0.21953f, 0.223595f, 0.227661f, 0.231726f, 0.235791f, 0.239857f,
		0.243922f, 0.247988f, 0.252053f, 0.256118f, 0.260184f, 0.264249f,
		0.268314f, 0.27238f, 0.276445f, 0.28051f, 0.284576f, 0.288641f,
		0.293033f, 0.299418f, 0.305803f, 0.312188f, 0.318573f, 0.324958f,
		0.331343f, 0.337728f, 0.344114f, 0.350499f, 0.356884f, 0.363269f,
		0.369654f, 0.376039f, 0.382424f, 0.388809f, 0.395194f, 0.401579f,
		0.407965f, 0.41435f, 0.420735f, 0.42712f, 0.433505f, 0.43989f,
		0.446275f, 0.45266f, 0.459045f, 0.46543f, 0.471816f, 0.478201f,
		0.484586f, 0.490971f, 0.497356f, 0.503741f, 0.510126f, 0.516511f,
		0.522896f, 0.529282f, 0.535667f, 0.542052f, 0.548437f, 0.554822f,
		0.561207f, 0.567592f, 0.573977f, 0.580362f, 0.586747f, 0.593133f,
		0.599518f, 0.605903f, 0.612288f, 0.618673f, 0.625058f, 0.631443f,
		0.637828f, 0.644213f, 0.650598f, 0.656983f, 0.663369f, 0.669754f,
		0.676139f, 0.682524f, 0.688909f, 0.695294f, 0.701679f, 0.708064f,
		0.71445f, 0.720473f, 0.725528f, 0.730583f, 0.735637f, 0.740692f,
		0.745746f, 0.750801f, 0.755856f, 0.76091f, 0.765965f, 0.771019f,
		0.776074f, 0.781128f, 0.786183f, 0.791238f, 0.796292f, 0.801347f,
		0.806401f, 0.811456f, 0.816511f, 0.821565f, 0.82662f, 0.831674f,
		0.836729f, 0.841783f, 0.846838f, 0.851893f, 0.856947f, 0.862002f,
		0.867056f, 0.872111f, 0.877166f, 0.88222f, 0.887275f, 0.892329f,
		0.897384f, 0.902439f, 0.907493f, 0.912548f, 0.917602f, 0.922657f,
		0.927711f, 0.932766f, 0.937821f, 0.942875f, 0.94793f, 0.952984f,
		0.958039f, 0.963094f, 0.966472f, 0.966961f, 0.967451f, 0.96794f,
		0.96843f, 0.968919f, 0.969409f, 0.969898f, 0.970388f, 0.970877f,
		0.971367f, 0.971856f, 0.972346f, 0.972835f, 0.973325f, 0.973814f,
		0.974303f, 0.974793f, 0.975282f, 0.975772f, 0.976261f, 0.976751f,
		0.97724f, 0.97773f, 0.978219f, 0.978709f, 0.979198f, 0.979688f,
		0.980177f, 0.980667f, 0.981156f, 0.981646f, 0.982135f, 0.982625f,
		0.983114f, 0.983604f, 0.984093f, 0.984582f, 0.985072f, 0.985561f,
		0.986051f, 0.98654f, 0.98703f, 0.987519f, 0.988009f, 0.988498f,
		0.988988f, 0.989477f, 0.989967f, 0.990456f, 0.990946f, 0.991435f,
		0.991925f, 0.992414f, 0.992904f, 0.993393f, 0.993883f, 0.994372f,
		0.994861f, 0.995351f, 0.99584f, 0.99633f, 0.996819f, 0.997309f,
		0.997798f, 0.998288f, 0.998777f, 0.999267f, 0.999756f, 0.997431f,
		0.992314f, 0.987197f, 0.98208f, 0.976962f, 0.971845f, 0.966728f,
		0.961611f, 0.956493f, 0.951376f, 0.946259f, 0.941142f, 0.936024f,
		0.930907f, 0.92579f, 0.920673f, 0.915555f, 0.910438f, 0.905321f,
		0.900204f, 0.895086f, 0.889969f, 0.884852f, 0.879735f, 0.874617f,
		0.8695f, 0.864383f, 0.859266f, 0.854148f, 0.849031f, 0.843914f,
		0.838797f, 0.833679f, 0.826997f, 0.819117f, 0.811236f, 0.803355f,
		0.795475f, 0.787594f, 0.779714f, 0.771833f, 0.763953f, 0.756072f,
		0.748191f, 0.740311f, 0.73243f, 0.72455f, 0.716669f, 0.708789f,
		0.700908f, 0.693028f, 0.685147f, 0.677266f, 0.669386f, 0.661505f,
		0.653625f, 0.645744f, 0.637864f, 0.629983f, 0.622102f, 0.614222f,
		0.606341f, 0.598461f, 0.59058f, 0.5827f, 0.574819f, 0.566939f,
		0.559058f, 0.551177f, 0.543297f, 0.535416f, 0.527536f, 0.519655f,
		0.511775f, 0.503894f, 0.496013f, 0.488133f, 0.480252f, 0.472372f,
		0.464491f, 0.456611f, 0.44873f, 0.44085f, 0.432969f, 0.425088f,
		0.417208f, 0.409327f, 0.401447f, 0.393566f, 0.385686f, 0.377805f,
		0.369924f, 0.362044f, 0.354163f, 0.346283f, 0.338402f, 0.330522f,
		0.322641f, 0.314761f, 0.30688f, 0.298999f, 0.291119f, 0.283238f,
		0.275358f, 0.267477f, 0.259597f, 0.251716f, 0.243835f, 0.235955f,
		0.228074f, 0.220194f, 0.212313f, 0.204433f, 0.200213f, 0.197399f,
		0.194584f, 0.19177f, 0.188955f, 0.186141f, 0.183326f, 0.180512f,
		0.177697f, 0.174883f, 0.172068f, 0.169254f, 0.166439f, 0.163625f,
		0.16081f, 0.157996f, 0.155182f, 0.152367f, 0.149553f, 0.146738f,
		0.143924f, 0.141109f, 0.138295f, 0.13548f, 0.132666f, 0.129851f,
		0.127037f, 0.124222f, 0.121408f, 0.118593f, 0.115779f, 0.112964f,
		0.11015f, 0.107335f, 0.104521f, 0.101706f, 0.0988918f, 0.0960773f,
		0.0932628f, 0.0904483f, 0.0876338f, 0.0848194f, 0.0820048f, 0.0791904f,
		0.0780014f, 0.0771973f, 0.0763932f, 0.0755891f, 0.0747849f, 0.0739807f,
		0.0731766f, 0.0723725f, 0.0715683f, 0.0707642f, 0.0699601f, 0.0691559f,
		0.0683518f, 0.0675477f, 0.0667435f, 0.0659394f, 0.0651352f, 0.0643311f,
		0.0635269f, 0.0627228f, 0.0619187f, 0.0611145f, 0.0603104f, 0.0595062f,
		0.0587021f, 0.057898f, 0.0570939f, 0.0562897f, 0.0554855f, 0.0546814f,
		0.0538773f, 0.0530732f, 0.052269f, 0.0514649f, 0.0506607f, 0.0498566f,
		0.0490525f, 0.0482484f, 0.0474442f, 0.04664f, 0.0458359f, 0.0450318f,
		0.0442276f, 0.0434235f, 0.0426193f, 0.0418152f, 0.041011f, 0.0402069f,
		0.0394028f, 0.0385987f, 0.0377945f, 0.0369904f, 0.0361862f, 0.0353821f,
		0.034578f, 0.0337738f, 0.0329697f, 0.0321655f, 0.0313614f, 0.0305573f,
		0.0297531f, 0.028949f, 0.0281448f, 0.0273407f, 0.0265366f, 0.0257325f,
		0.0249283f, 0.0241241f, 0.02332f, 0.0225159f, 0.0217117f, 0.0209076f,
		0.0201035f, 0.0192993f, 0.0184952f, 0.0176911f, 0.0168869f, 0.0160828f,
		0.0152786f, 0.0144745f, 0.0136703f, 0.0128662f, 0.0120621f, 0.0112579f,
		0.0104538f, 0.00964969f, 0.00884551f, 0.00804138f, 0.00723726f, 0.00643313f,
		0.00562894f, 0.00482482f, 0.00402069f, 0.00321656f, 0.00241244f, 0.00160825f,
		0.000804126f, 0.f
	};
};

template <int NV>
using cable_table_t = wrap::data<control::cable_table<cable_table_mod<NV>>, 
                                 data::embedded::table<cable_table_t_data>>;
template <int NV> using dry_wet_mixer2_c1_0 = dry_wet_mixer2_c0<NV>;

template <int NV>
using dry_wet_mixer2_c1 = parameter::chain<ranges::Identity, 
                                           dry_wet_mixer2_c1_0<NV>, 
                                           parameter::plain<cable_table_t<NV>, 0>>;

template <int NV>
using dry_wet_mixer2_multimod = parameter::list<dry_wet_mixer2_c0<NV>, dry_wet_mixer2_c1<NV>>;

template <int NV>
using dry_wet_mixer2_t = control::xfader<dry_wet_mixer2_multimod<NV>, 
                                         faders::cosine>;

template <int NV>
using dry_path2_t = container::chain<parameter::empty, 
                                     wrap::fix<2, dry_wet_mixer2_t<NV>>, 
                                     core::gain<NV>>;

template <int NV>
using wet_path2_t = container::chain<parameter::empty, 
                                     wrap::fix<2, project::granular<NV>>, 
                                     core::gain<NV>, 
                                     core::gain<NV>, 
                                     cable_table_t<NV>>;

namespace dry_wet3_t_parameters
{
}

template <int NV>
using dry_wet3_t = container::split<parameter::plain<mod_fx_impl::dry_wet_mixer2_t<NV>, 0>, 
                                    wrap::fix<2, dry_path2_t<NV>>, 
                                    wet_path2_t<NV>>;

template <int NV>
using wrapsmoothed_parameter11_t = container::chain<parameter::empty, 
                                                    wrap::fix<2, tempo_sync_t<NV>>, 
                                                    dry_wet3_t<NV>, 
                                                    pma_t<NV>, 
                                                    smoothed_parameter1_t<NV>>;

template <int NV> using xfader_c0 = dry_wet_mixer2_c0<NV>;

template <int NV> using xfader_c1_0 = dry_wet_mixer2_c0<NV>;

template <int NV> using xfader_c1_1 = dry_wet_mixer2_c0<NV>;

template <int NV>
using xfader_c1 = parameter::chain<ranges::Identity, 
                                   xfader_c1_0<NV>, 
                                   xfader_c1_1<NV>>;

template <int NV>
using xfader_multimod = parameter::list<xfader_c0<NV>, xfader_c1<NV>>;

template <int NV>
using xfader_t = control::xfader<xfader_multimod<NV>, faders::overlap>;

template <int NV>
using tempo_sync8_t = wrap::mod<parameter::plain<jdsp::jdelay<NV>, 1>, 
                                control::tempo_sync<NV>>;
template <int NV>
using mono_cable = cable::block<NV, 1>;

template <int NV> using dry_wet_mixer_c0 = dry_wet_mixer2_c0<NV>;

template <int NV> using dry_wet_mixer_c1 = dry_wet_mixer2_c0<NV>;

template <int NV>
using dry_wet_mixer_multimod = parameter::list<dry_wet_mixer_c0<NV>, dry_wet_mixer_c1<NV>>;

template <int NV>
using dry_wet_mixer_t = control::xfader<dry_wet_mixer_multimod<NV>, 
                                        faders::cosine>;

template <int NV>
using dry_path_t = container::chain<parameter::empty, 
                                    wrap::fix<1, dry_wet_mixer_t<NV>>, 
                                    core::gain<NV>>;

using simple_ar_multimod = parameter::list<parameter::empty, parameter::empty>;

template <int NV>
using simple_ar_t = wrap::no_data<envelope::simple_ar<NV, simple_ar_multimod>>;

template <int NV>
using wet_path_t = container::chain<parameter::empty, 
                                    wrap::fix<1, simple_ar_t<NV>>, 
                                    core::gain<NV>>;

namespace dry_wet1_t_parameters
{
}

template <int NV>
using dry_wet1_t = container::split<parameter::plain<mod_fx_impl::dry_wet_mixer_t<NV>, 0>, 
                                    wrap::fix<1, dry_path_t<NV>>, 
                                    wet_path_t<NV>>;

using simple_ar1_multimod = simple_ar_multimod;

template <int NV>
using simple_ar1_t = wrap::no_data<envelope::simple_ar<NV, simple_ar1_multimod>>;

template <int NV>
using clock_ramp1_mod = parameter::chain<ranges::Identity, 
                                         parameter::plain<simple_ar_t<NV>, 2>, 
                                         parameter::plain<simple_ar1_t<NV>, 2>>;

template <int NV>
using clock_ramp1_t = wrap::mod<clock_ramp1_mod<NV>, 
                                wrap::no_data<core::clock_ramp<NV, false>>>;

template <int NV>
using chain3_t = container::chain<parameter::empty, 
                                  wrap::fix<1, tempo_sync8_t<NV>>, 
                                  core::gain<NV>, 
                                  routing::receive<NV, mono_cable<NV>>, 
                                  jdsp::jdelay<NV>, 
                                  routing::send<NV, mono_cable<NV>>, 
                                  dry_wet1_t<NV>, 
                                  clock_ramp1_t<NV>>;

template <int NV> using tempo_sync9_t = tempo_sync8_t<NV>;

template <int NV> using dry_wet_mixer1_c0 = dry_wet_mixer2_c0<NV>;

template <int NV> using dry_wet_mixer1_c1 = dry_wet_mixer2_c0<NV>;

template <int NV>
using dry_wet_mixer1_multimod = parameter::list<dry_wet_mixer1_c0<NV>, dry_wet_mixer1_c1<NV>>;

template <int NV>
using dry_wet_mixer1_t = control::xfader<dry_wet_mixer1_multimod<NV>, 
                                         faders::cosine>;

template <int NV>
using dry_path1_t = container::chain<parameter::empty, 
                                     wrap::fix<1, dry_wet_mixer1_t<NV>>, 
                                     core::gain<NV>>;

template <int NV>
using wet_path1_t = container::chain<parameter::empty, 
                                     wrap::fix<1, simple_ar1_t<NV>>, 
                                     core::gain<NV>>;

namespace dry_wet2_t_parameters
{
}

template <int NV>
using dry_wet2_t = container::split<parameter::plain<mod_fx_impl::dry_wet_mixer1_t<NV>, 0>, 
                                    wrap::fix<1, dry_path1_t<NV>>, 
                                    wet_path1_t<NV>>;

template <int NV>
using chain1_t = container::chain<parameter::empty, 
                                  wrap::fix<1, tempo_sync9_t<NV>>, 
                                  core::gain<NV>, 
                                  routing::receive<NV, mono_cable<NV>>, 
                                  jdsp::jdelay<NV>, 
                                  routing::send<NV, mono_cable<NV>>, 
                                  dry_wet2_t<NV>>;

template <int NV>
using multi_t = container::multi<parameter::empty, 
                                 wrap::fix<1, chain3_t<NV>>, 
                                 wrap::fix<1, chain1_t<NV>>>;

template <int NV>
using split_t = container::split<parameter::empty, 
                                 wrap::fix<2, core::gain<NV>>, 
                                 multi_t<NV>>;

template <int NV>
using delay_chain_t = container::chain<parameter::empty, 
                                       wrap::fix<2, xfader_t<NV>>, 
                                       split_t<NV>>;
using limiter_t = wrap::no_data<dynamics::limiter>;

namespace mod_fx_t_parameters
{
// Parameter list for mod_fx_impl::mod_fx_t --------------------------------------------------------

DECLARE_PARAMETER_RANGE_STEP(Pitch_InputRange, 
                             -12., 
                             12., 
                             1.);
DECLARE_PARAMETER_RANGE(Pitch_0Range, 
                        -12., 
                        12.);

template <int NV>
using Pitch_0 = parameter::from0To1<project::granular<NV>, 
                                    3, 
                                    Pitch_0Range>;

template <int NV>
using Pitch = parameter::chain<Pitch_InputRange, Pitch_0<NV>>;

DECLARE_PARAMETER_RANGE(Engine1DryWet_InputRange, 
                        0., 
                        100.);

template <int NV>
using Engine1DryWet = parameter::chain<Engine1DryWet_InputRange, 
                                       parameter::plain<mod_fx_impl::dry_wet3_t<NV>, 0>>;

DECLARE_PARAMETER_RANGE(Delay_Mix_InputRange, 
                        0., 
                        100.);

template <int NV>
using Delay_Mix = parameter::chain<Delay_Mix_InputRange, 
                                   parameter::plain<mod_fx_impl::xfader_t<NV>, 0>>;

template <int NV>
using Delay_Sync = parameter::chain<ranges::Identity, 
                                    parameter::plain<mod_fx_impl::tempo_sync9_t<NV>, 2>, 
                                    parameter::plain<mod_fx_impl::tempo_sync8_t<NV>, 2>>;

template <int NV>
using Delay_Feedback = parameter::chain<ranges::Identity, 
                                        parameter::plain<routing::receive<NV, mono_cable<NV>>, 0>, 
                                        parameter::plain<routing::receive<NV, mono_cable<NV>>, 0>>;

template <int NV>
using Accent = parameter::chain<ranges::Identity, 
                                parameter::plain<mod_fx_impl::dry_wet1_t<NV>, 0>, 
                                parameter::plain<mod_fx_impl::dry_wet2_t<NV>, 0>>;

DECLARE_PARAMETER_RANGE(SpeedA_InputRange, 
                        10., 
                        500.);
DECLARE_PARAMETER_RANGE_STEP(SpeedA_0Range, 
                             10., 
                             500., 
                             0.1);

template <int NV>
using SpeedA_0 = parameter::from0To1<mod_fx_impl::tempo_sync_t<NV>, 
                                     3, 
                                     SpeedA_0Range>;

template <int NV>
using SpeedA = parameter::chain<SpeedA_InputRange, SpeedA_0<NV>>;

DECLARE_PARAMETER_RANGE(SpeedB_InputRange, 
                        0., 
                        18.);
DECLARE_PARAMETER_RANGE_STEP(SpeedB_0Range, 
                             0., 
                             18., 
                             1.);

template <int NV>
using SpeedB_0 = parameter::from0To1<mod_fx_impl::tempo_sync_t<NV>, 
                                     0, 
                                     SpeedB_0Range>;

template <int NV>
using SpeedB = parameter::chain<SpeedB_InputRange, SpeedB_0<NV>>;

template <int NV>
using ModSpeed = parameter::plain<mod_fx_impl::clock_ramp_t<NV>, 
                                  0>;
template <int NV>
using TempoSync = parameter::plain<mod_fx_impl::tempo_sync_t<NV>, 
                                   2>;
template <int NV>
using GrainSize = parameter::plain<project::granular<NV>, 5>;
template <int NV>
using Offset = parameter::plain<project::granular<NV>, 4>;
template <int NV>
using SpeedL = parameter::plain<mod_fx_impl::tempo_sync8_t<NV>, 
                                0>;
template <int NV>
using SpeedR = parameter::plain<mod_fx_impl::tempo_sync9_t<NV>, 
                                0>;
template <int NV>
using Speed_LA = parameter::plain<mod_fx_impl::tempo_sync8_t<NV>, 
                                  3>;
template <int NV>
using Speed_RA = parameter::plain<mod_fx_impl::tempo_sync9_t<NV>, 
                                  3>;
template <int NV>
using mod_fx_t_plist = parameter::list<ModSpeed<NV>, 
                                       TempoSync<NV>, 
                                       GrainSize<NV>, 
                                       Pitch<NV>, 
                                       Offset<NV>, 
                                       Engine1DryWet<NV>, 
                                       Delay_Mix<NV>, 
                                       Delay_Sync<NV>, 
                                       Delay_Feedback<NV>, 
                                       SpeedL<NV>, 
                                       SpeedR<NV>, 
                                       Accent<NV>, 
                                       SpeedA<NV>, 
                                       SpeedB<NV>, 
                                       Speed_LA<NV>, 
                                       Speed_RA<NV>>;
}

template <int NV>
using mod_fx_t_ = container::chain<mod_fx_t_parameters::mod_fx_t_plist<NV>, 
                                   wrap::fix<2, smoothed_parameter2_t<NV>>, 
                                   mods_t<NV>, 
                                   pack2_writer_t, 
                                   wrapsmoothed_parameter11_t<NV>, 
                                   delay_chain_t<NV>, 
                                   limiter_t>;

// =================================| Root node initialiser class |=================================

template <int NV> struct instance: public mod_fx_impl::mod_fx_t_<NV>
{
	
	struct metadata
	{
		static const int NumTables = 0;
		static const int NumSliderPacks = 3;
		static const int NumAudioFiles = 0;
		static const int NumFilters = 0;
		static const int NumDisplayBuffers = 0;
		
		SNEX_METADATA_ID(mod_fx);
		SNEX_METADATA_NUM_CHANNELS(2);
		SNEX_METADATA_ENCODED_PARAMETERS(282)
		{
			0x005C, 0x0000, 0x0000, 0x6F4D, 0x5364, 0x6570, 0x6465, 0x0000, 
            0x0000, 0x0000, 0x9000, 0x0041, 0x9000, 0x0041, 0x8000, 0x003F, 
            0x8000, 0x5C3F, 0x0100, 0x0000, 0x5400, 0x6D65, 0x6F70, 0x7953, 
            0x636E, 0x0000, 0x0000, 0x0000, 0x8000, 0x003F, 0x0000, 0x0000, 
            0x8000, 0x003F, 0x8000, 0x5C3F, 0x0200, 0x0000, 0x4700, 0x6172, 
            0x6E69, 0x6953, 0x657A, 0x0000, 0x2000, 0x0041, 0x1600, 0x0043, 
            0x1600, 0x0043, 0x8000, 0x003F, 0x0000, 0x5C00, 0x0300, 0x0000, 
            0x5000, 0x7469, 0x6863, 0x0000, 0x4000, 0x00C1, 0x4000, 0x0041, 
            0x4000, 0x0041, 0x8000, 0x003F, 0x8000, 0x5C3F, 0x0400, 0x0000, 
            0x4F00, 0x6666, 0x6573, 0x0074, 0x0000, 0x4120, 0x0000, 0x43FA, 
            0x0000, 0x4120, 0x0000, 0x3F80, 0x0000, 0x0000, 0x005C, 0x0005, 
            0x0000, 0x6E45, 0x6967, 0x656E, 0x4431, 0x7972, 0x6557, 0x0074, 
            0x0000, 0x0000, 0x0000, 0x42C8, 0x999A, 0x4241, 0x0000, 0x3F80, 
            0x0000, 0x0000, 0x005C, 0x0006, 0x0000, 0x6544, 0x616C, 0x5F79, 
            0x694D, 0x0078, 0x0000, 0x0000, 0x0000, 0x42C8, 0x0000, 0x41C8, 
            0x0000, 0x3F80, 0x0000, 0x0000, 0x005C, 0x0007, 0x0000, 0x6544, 
            0x616C, 0x5F79, 0x7953, 0x636E, 0x0000, 0x0000, 0x0000, 0x8000, 
            0x003F, 0x8000, 0x003F, 0x8000, 0x003F, 0x8000, 0x5C3F, 0x0800, 
            0x0000, 0x4400, 0x6C65, 0x7961, 0x465F, 0x6565, 0x6264, 0x6361, 
            0x006B, 0x0000, 0x0000, 0x0000, 0x3F80, 0x147B, 0x3F2E, 0x0000, 
            0x3F80, 0x0000, 0x0000, 0x005C, 0x0009, 0x0000, 0x7053, 0x6565, 
            0x4C64, 0x0000, 0x0000, 0x0000, 0x9000, 0x0041, 0x0000, 0x0000, 
            0x8000, 0x003F, 0x8000, 0x5C3F, 0x0A00, 0x0000, 0x5300, 0x6570, 
            0x6465, 0x0052, 0x0000, 0x0000, 0x0000, 0x4190, 0x0000, 0x0000, 
            0x0000, 0x3F80, 0x0000, 0x3F80, 0x005C, 0x000B, 0x0000, 0x6341, 
            0x6563, 0x746E, 0x0000, 0x0000, 0x0000, 0x8000, 0xCD3F, 0x4CCC, 
            0x003E, 0x8000, 0x003F, 0x0000, 0x5C00, 0x0C00, 0x0000, 0x5300, 
            0x6570, 0x6465, 0x0041, 0x0000, 0x4120, 0x0000, 0x43FA, 0x0000, 
            0x4130, 0x0000, 0x3F80, 0x0000, 0x0000, 0x005C, 0x000D, 0x0000, 
            0x7053, 0x6565, 0x4264, 0x0000, 0x0000, 0x0000, 0x9000, 0x0041, 
            0x0000, 0x0000, 0x8000, 0x003F, 0x0000, 0x5C00, 0x0E00, 0x0000, 
            0x5300, 0x6570, 0x6465, 0x4C5F, 0x0041, 0x0000, 0x0000, 0x0000, 
            0x447A, 0x0000, 0x0000, 0x0000, 0x3F80, 0x0000, 0x3F80, 0x005C, 
            0x000F, 0x0000, 0x7053, 0x6565, 0x5F64, 0x4152, 0x0000, 0x0000, 
            0x0000, 0x7A00, 0x0044, 0x0000, 0x0000, 0x8000, 0x003F, 0x8000, 
            0x003F, 0x0000
		};
		SNEX_METADATA_ENCODED_MOD_INFO(17)
		{
			0x003A, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
            0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
            0x0000
		};
	};
	
	instance()
	{
		// Node References -------------------------------------------------------------------------
		
		auto& smoothed_parameter2 = this->getT(0);                                            // mod_fx_impl::smoothed_parameter2_t<NV>
		auto& mods = this->getT(1);                                                           // mod_fx_impl::mods_t<NV>
		auto& modchain = this->getT(1).getT(0);                                               // mod_fx_impl::modchain_t<NV>
		auto& clock_ramp = this->getT(1).getT(0).getT(0);                                     // mod_fx_impl::clock_ramp_t<NV>
		auto& cable_pack = this->getT(1).getT(1);                                             // mod_fx_impl::cable_pack_t<NV>
		auto& smoothed_parameter = this->getT(1).getT(2);                                     // mod_fx_impl::smoothed_parameter_t<NV>
		auto& pack2_writer = this->getT(2);                                                   // mod_fx_impl::pack2_writer_t
		auto& wrapsmoothed_parameter11 = this->getT(3);                                       // mod_fx_impl::wrapsmoothed_parameter11_t<NV>
		auto& tempo_sync = this->getT(3).getT(0);                                             // mod_fx_impl::tempo_sync_t<NV>
		auto& dry_wet3 = this->getT(3).getT(1);                                               // mod_fx_impl::dry_wet3_t<NV>
		auto& dry_path2 = this->getT(3).getT(1).getT(0);                                      // mod_fx_impl::dry_path2_t<NV>
		auto& dry_wet_mixer2 = this->getT(3).getT(1).getT(0).getT(0);                         // mod_fx_impl::dry_wet_mixer2_t<NV>
		auto& dry_gain2 = this->getT(3).getT(1).getT(0).getT(1);                              // core::gain<NV>
		auto& wet_path2 = this->getT(3).getT(1).getT(1);                                      // mod_fx_impl::wet_path2_t<NV>
		auto& granular = this->getT(3).getT(1).getT(1).getT(0);                               // project::granular<NV>
		auto& wet_gain2 = this->getT(3).getT(1).getT(1).getT(1);                              // core::gain<NV>
		auto& gain = this->getT(3).getT(1).getT(1).getT(2);                                   // core::gain<NV>
		auto& cable_table = this->getT(3).getT(1).getT(1).getT(3);                            // mod_fx_impl::cable_table_t<NV>
		auto& pma = this->getT(3).getT(2);                                                    // mod_fx_impl::pma_t<NV>
		auto& smoothed_parameter1 = this->getT(3).getT(3);                                    // mod_fx_impl::smoothed_parameter1_t<NV>
		auto& delay_chain = this->getT(4);                                                    // mod_fx_impl::delay_chain_t<NV>
		auto& xfader = this->getT(4).getT(0);                                                 // mod_fx_impl::xfader_t<NV>
		auto& split = this->getT(4).getT(1);                                                  // mod_fx_impl::split_t<NV>
		auto& gain7 = this->getT(4).getT(1).getT(0);                                          // core::gain<NV>
		auto& multi = this->getT(4).getT(1).getT(1);                                          // mod_fx_impl::multi_t<NV>
		auto& chain3 = this->getT(4).getT(1).getT(1).getT(0);                                 // mod_fx_impl::chain3_t<NV>
		auto& tempo_sync8 = this->getT(4).getT(1).getT(1).getT(0).getT(0);                    // mod_fx_impl::tempo_sync8_t<NV>
		auto& gain8 = this->getT(4).getT(1).getT(1).getT(0).getT(1);                          // core::gain<NV>
		auto& receive = this->getT(4).getT(1).getT(1).getT(0).getT(2);                        // routing::receive<NV, mono_cable<NV>>
		auto& jdelay1 = this->getT(4).getT(1).getT(1).getT(0).getT(3);                        // jdsp::jdelay<NV>
		auto& send1 = this->getT(4).getT(1).getT(1).getT(0).getT(4);                          // routing::send<NV, mono_cable<NV>>
		auto& dry_wet1 = this->getT(4).getT(1).getT(1).getT(0).getT(5);                       // mod_fx_impl::dry_wet1_t<NV>
		auto& dry_path = this->getT(4).getT(1).getT(1).getT(0).getT(5).getT(0);               // mod_fx_impl::dry_path_t<NV>
		auto& dry_wet_mixer = this->getT(4).getT(1).getT(1).getT(0).getT(5).getT(0).getT(0);  // mod_fx_impl::dry_wet_mixer_t<NV>
		auto& dry_gain = this->getT(4).getT(1).getT(1).getT(0).getT(5).getT(0).getT(1);       // core::gain<NV>
		auto& wet_path = this->getT(4).getT(1).getT(1).getT(0).getT(5).getT(1);               // mod_fx_impl::wet_path_t<NV>
		auto& simple_ar = this->getT(4).getT(1).getT(1).getT(0).getT(5).getT(1).getT(0);      // mod_fx_impl::simple_ar_t<NV>
		auto& wet_gain = this->getT(4).getT(1).getT(1).getT(0).getT(5).getT(1).getT(1);       // core::gain<NV>
		auto& clock_ramp1 = this->getT(4).getT(1).getT(1).getT(0).getT(6);                    // mod_fx_impl::clock_ramp1_t<NV>
		auto& chain1 = this->getT(4).getT(1).getT(1).getT(1);                                 // mod_fx_impl::chain1_t<NV>
		auto& tempo_sync9 = this->getT(4).getT(1).getT(1).getT(1).getT(0);                    // mod_fx_impl::tempo_sync9_t<NV>
		auto& gain9 = this->getT(4).getT(1).getT(1).getT(1).getT(1);                          // core::gain<NV>
		auto& receive1 = this->getT(4).getT(1).getT(1).getT(1).getT(2);                       // routing::receive<NV, mono_cable<NV>>
		auto& jdelay = this->getT(4).getT(1).getT(1).getT(1).getT(3);                         // jdsp::jdelay<NV>
		auto& send = this->getT(4).getT(1).getT(1).getT(1).getT(4);                           // routing::send<NV, mono_cable<NV>>
		auto& dry_wet2 = this->getT(4).getT(1).getT(1).getT(1).getT(5);                       // mod_fx_impl::dry_wet2_t<NV>
		auto& dry_path1 = this->getT(4).getT(1).getT(1).getT(1).getT(5).getT(0);              // mod_fx_impl::dry_path1_t<NV>
		auto& dry_wet_mixer1 = this->getT(4).getT(1).getT(1).getT(1).getT(5).getT(0).getT(0); // mod_fx_impl::dry_wet_mixer1_t<NV>
		auto& dry_gain1 = this->getT(4).getT(1).getT(1).getT(1).getT(5).getT(0).getT(1);      // core::gain<NV>
		auto& wet_path1 = this->getT(4).getT(1).getT(1).getT(1).getT(5).getT(1);              // mod_fx_impl::wet_path1_t<NV>
		auto& simple_ar1 = this->getT(4).getT(1).getT(1).getT(1).getT(5).getT(1).getT(0);     // mod_fx_impl::simple_ar1_t<NV>
		auto& wet_gain1 = this->getT(4).getT(1).getT(1).getT(1).getT(5).getT(1).getT(1);      // core::gain<NV>
		auto& limiter = this->getT(5);                                                        // mod_fx_impl::limiter_t
		
		// Parameter Connections -------------------------------------------------------------------
		
		dry_wet3.getParameterT(0).connectT(0, dry_wet_mixer2); // DryWet -> dry_wet_mixer2::Value
		dry_wet1.getParameterT(0).connectT(0, dry_wet_mixer);  // DryWet -> dry_wet_mixer::Value
		dry_wet2.getParameterT(0).connectT(0, dry_wet_mixer1); // DryWet -> dry_wet_mixer1::Value
		this->getParameterT(0).connectT(0, clock_ramp);        // ModSpeed -> clock_ramp::Tempo
		
		this->getParameterT(1).connectT(0, tempo_sync); // TempoSync -> tempo_sync::Enabled
		
		this->getParameterT(2).connectT(0, granular); // GrainSize -> granular::grainSize
		
		this->getParameterT(3).connectT(0, granular); // Pitch -> granular::pitch
		
		this->getParameterT(4).connectT(0, granular); // Offset -> granular::grainPos
		
		this->getParameterT(5).connectT(0, dry_wet3); // Engine1DryWet -> dry_wet3::DryWet
		
		this->getParameterT(6).connectT(0, xfader); // Delay_Mix -> xfader::Value
		
		auto& Delay_Sync_p = this->getParameterT(7);
		Delay_Sync_p.connectT(0, tempo_sync9); // Delay_Sync -> tempo_sync9::Enabled
		Delay_Sync_p.connectT(1, tempo_sync8); // Delay_Sync -> tempo_sync8::Enabled
		
		auto& Delay_Feedback_p = this->getParameterT(8);
		Delay_Feedback_p.connectT(0, receive);  // Delay_Feedback -> receive::Feedback
		Delay_Feedback_p.connectT(1, receive1); // Delay_Feedback -> receive1::Feedback
		
		this->getParameterT(9).connectT(0, tempo_sync8); // SpeedL -> tempo_sync8::Tempo
		
		this->getParameterT(10).connectT(0, tempo_sync9); // SpeedR -> tempo_sync9::Tempo
		
		auto& Accent_p = this->getParameterT(11);
		Accent_p.connectT(0, dry_wet1); // Accent -> dry_wet1::DryWet
		Accent_p.connectT(1, dry_wet2); // Accent -> dry_wet2::DryWet
		
		this->getParameterT(12).connectT(0, tempo_sync); // SpeedA -> tempo_sync::UnsyncedTime
		
		this->getParameterT(13).connectT(0, tempo_sync); // SpeedB -> tempo_sync::Tempo
		
		this->getParameterT(14).connectT(0, tempo_sync8); // Speed_LA -> tempo_sync8::UnsyncedTime
		
		this->getParameterT(15).connectT(0, tempo_sync9); // Speed_RA -> tempo_sync9::UnsyncedTime
		
		// Modulation Connections ------------------------------------------------------------------
		
		smoothed_parameter2.getParameter().connectT(0, pack2_writer);                  // smoothed_parameter2 -> pack2_writer::Value1
		smoothed_parameter.getParameter().connectT(0, pack2_writer);                   // smoothed_parameter -> pack2_writer::Value2
		pma.getWrappedObject().getParameter().connectT(0, granular);                   // pma -> granular::mix
		smoothed_parameter1.getParameter().connectT(0, pma);                           // smoothed_parameter1 -> pma::Value
		cable_pack.getWrappedObject().getParameter().connectT(0, smoothed_parameter);  // cable_pack -> smoothed_parameter::Value
		cable_pack.getWrappedObject().getParameter().connectT(1, smoothed_parameter1); // cable_pack -> smoothed_parameter1::Value
		clock_ramp.getParameter().connectT(0, cable_pack);                             // clock_ramp -> cable_pack::Value
		clock_ramp.getParameter().connectT(1, smoothed_parameter2);                    // clock_ramp -> smoothed_parameter2::Value
		tempo_sync.getParameter().connectT(0, granular);                               // tempo_sync -> granular::interval
		cable_table.getWrappedObject().getParameter().connectT(0, gain);               // cable_table -> gain::Gain
		auto& dry_wet_mixer2_p = dry_wet_mixer2.getWrappedObject().getParameter();
		dry_wet_mixer2_p.getParameterT(0).connectT(0, dry_gain2);   // dry_wet_mixer2 -> dry_gain2::Gain
		dry_wet_mixer2_p.getParameterT(1).connectT(0, wet_gain2);   // dry_wet_mixer2 -> wet_gain2::Gain
		dry_wet_mixer2_p.getParameterT(1).connectT(1, cable_table); // dry_wet_mixer2 -> cable_table::Value
		auto& xfader_p = xfader.getWrappedObject().getParameter();
		xfader_p.getParameterT(0).connectT(0, gain7);    // xfader -> gain7::Gain
		xfader_p.getParameterT(1).connectT(0, gain8);    // xfader -> gain8::Gain
		xfader_p.getParameterT(1).connectT(1, gain9);    // xfader -> gain9::Gain
		tempo_sync8.getParameter().connectT(0, jdelay1); // tempo_sync8 -> jdelay1::DelayTime
		auto& dry_wet_mixer_p = dry_wet_mixer.getWrappedObject().getParameter();
		dry_wet_mixer_p.getParameterT(0).connectT(0, dry_gain); // dry_wet_mixer -> dry_gain::Gain
		dry_wet_mixer_p.getParameterT(1).connectT(0, wet_gain); // dry_wet_mixer -> wet_gain::Gain
		auto& simple_ar_p = simple_ar.getWrappedObject().getParameter();
		auto& simple_ar1_p = simple_ar1.getWrappedObject().getParameter();
		clock_ramp1.getParameter().connectT(0, simple_ar);  // clock_ramp1 -> simple_ar::Gate
		clock_ramp1.getParameter().connectT(1, simple_ar1); // clock_ramp1 -> simple_ar1::Gate
		tempo_sync9.getParameter().connectT(0, jdelay);     // tempo_sync9 -> jdelay::DelayTime
		auto& dry_wet_mixer1_p = dry_wet_mixer1.getWrappedObject().getParameter();
		dry_wet_mixer1_p.getParameterT(0).connectT(0, dry_gain1); // dry_wet_mixer1 -> dry_gain1::Gain
		dry_wet_mixer1_p.getParameterT(1).connectT(0, wet_gain1); // dry_wet_mixer1 -> wet_gain1::Gain
		
		// Send Connections ------------------------------------------------------------------------
		
		send1.connect(receive1);
		send.connect(receive);
		
		// Default Values --------------------------------------------------------------------------
		
		;                                           // smoothed_parameter2::Value is automated
		smoothed_parameter2.setParameterT(1, 208.); // control::smoothed_parameter::SmoothingTime
		smoothed_parameter2.setParameterT(2, 1.);   // control::smoothed_parameter::Enabled
		
		;                                // clock_ramp::Tempo is automated
		clock_ramp.setParameterT(1, 4.); // core::clock_ramp::Multiplier
		clock_ramp.setParameterT(2, 0.); // core::clock_ramp::AddToSignal
		clock_ramp.setParameterT(3, 1.); // core::clock_ramp::UpdateMode
		clock_ramp.setParameterT(4, 0.); // core::clock_ramp::Inactive
		
		; // cable_pack::Value is automated
		
		;                                           // smoothed_parameter::Value is automated
		smoothed_parameter.setParameterT(1, 212.1); // control::smoothed_parameter::SmoothingTime
		smoothed_parameter.setParameterT(2, 1.);    // control::smoothed_parameter::Enabled
		
		; // pack2_writer::Value1 is automated
		; // pack2_writer::Value2 is automated
		
		;                                // tempo_sync::Tempo is automated
		tempo_sync.setParameterT(1, 1.); // control::tempo_sync::Multiplier
		;                                // tempo_sync::Enabled is automated
		;                                // tempo_sync::UnsyncedTime is automated
		
		; // dry_wet3::DryWet is automated
		
		; // dry_wet_mixer2::Value is automated
		
		;                                // dry_gain2::Gain is automated
		dry_gain2.setParameterT(1, 20.); // core::gain::Smoothing
		dry_gain2.setParameterT(2, 0.);  // core::gain::ResetValue
		
		granular.setParameterT(0, 100.); // project::granular::width
		granular.setParameterT(1, 12.);  // project::granular::gain
		;                                // granular::mix is automated
		;                                // granular::pitch is automated
		;                                // granular::grainPos is automated
		;                                // granular::grainSize is automated
		;                                // granular::interval is automated
		
		;                                // wet_gain2::Gain is automated
		wet_gain2.setParameterT(1, 20.); // core::gain::Smoothing
		wet_gain2.setParameterT(2, 0.);  // core::gain::ResetValue
		
		;                           // gain::Gain is automated
		gain.setParameterT(1, 20.); // core::gain::Smoothing
		gain.setParameterT(2, 0.);  // core::gain::ResetValue
		
		; // cable_table::Value is automated
		
		;                           // pma::Value is automated
		pma.setParameterT(1, 1.);   // control::pma::Multiply
		pma.setParameterT(2, 0.33); // control::pma::Add
		
		;                                            // smoothed_parameter1::Value is automated
		smoothed_parameter1.setParameterT(1, 109.2); // control::smoothed_parameter::SmoothingTime
		smoothed_parameter1.setParameterT(2, 1.);    // control::smoothed_parameter::Enabled
		
		; // xfader::Value is automated
		
		;                            // gain7::Gain is automated
		gain7.setParameterT(1, 20.); // core::gain::Smoothing
		gain7.setParameterT(2, 0.);  // core::gain::ResetValue
		
		;                                 // tempo_sync8::Tempo is automated
		tempo_sync8.setParameterT(1, 1.); // control::tempo_sync::Multiplier
		;                                 // tempo_sync8::Enabled is automated
		;                                 // tempo_sync8::UnsyncedTime is automated
		
		;                            // gain8::Gain is automated
		gain8.setParameterT(1, 20.); // core::gain::Smoothing
		gain8.setParameterT(2, 0.);  // core::gain::ResetValue
		
		; // receive::Feedback is automated
		
		jdelay1.setParameterT(0, 1000.); // jdsp::jdelay::Limit
		;                                // jdelay1::DelayTime is automated
		
		; // dry_wet1::DryWet is automated
		
		; // dry_wet_mixer::Value is automated
		
		;                               // dry_gain::Gain is automated
		dry_gain.setParameterT(1, 20.); // core::gain::Smoothing
		dry_gain.setParameterT(2, 0.);  // core::gain::ResetValue
		
		simple_ar.setParameterT(0, 50.9);  // envelope::simple_ar::Attack
		simple_ar.setParameterT(1, 116.8); // envelope::simple_ar::Release
		;                                  // simple_ar::Gate is automated
		simple_ar.setParameterT(3, 0.);    // envelope::simple_ar::AttackCurve
		
		;                               // wet_gain::Gain is automated
		wet_gain.setParameterT(1, 20.); // core::gain::Smoothing
		wet_gain.setParameterT(2, 0.);  // core::gain::ResetValue
		
		clock_ramp1.setParameterT(0, 11.); // core::clock_ramp::Tempo
		clock_ramp1.setParameterT(1, 1.);  // core::clock_ramp::Multiplier
		clock_ramp1.setParameterT(2, 0.);  // core::clock_ramp::AddToSignal
		clock_ramp1.setParameterT(3, 1.);  // core::clock_ramp::UpdateMode
		clock_ramp1.setParameterT(4, 2.);  // core::clock_ramp::Inactive
		
		;                                 // tempo_sync9::Tempo is automated
		tempo_sync9.setParameterT(1, 1.); // control::tempo_sync::Multiplier
		;                                 // tempo_sync9::Enabled is automated
		;                                 // tempo_sync9::UnsyncedTime is automated
		
		;                            // gain9::Gain is automated
		gain9.setParameterT(1, 20.); // core::gain::Smoothing
		gain9.setParameterT(2, 0.);  // core::gain::ResetValue
		
		; // receive1::Feedback is automated
		
		jdelay.setParameterT(0, 1000.); // jdsp::jdelay::Limit
		;                               // jdelay::DelayTime is automated
		
		; // dry_wet2::DryWet is automated
		
		; // dry_wet_mixer1::Value is automated
		
		;                                // dry_gain1::Gain is automated
		dry_gain1.setParameterT(1, 20.); // core::gain::Smoothing
		dry_gain1.setParameterT(2, 0.);  // core::gain::ResetValue
		
		simple_ar1.setParameterT(0, 29.4);  // envelope::simple_ar::Attack
		simple_ar1.setParameterT(1, 101.4); // envelope::simple_ar::Release
		;                                   // simple_ar1::Gate is automated
		simple_ar1.setParameterT(3, 0.);    // envelope::simple_ar::AttackCurve
		
		;                                // wet_gain1::Gain is automated
		wet_gain1.setParameterT(1, 20.); // core::gain::Smoothing
		wet_gain1.setParameterT(2, 0.);  // core::gain::ResetValue
		
		limiter.setParameterT(0, -3);  // dynamics::limiter::Threshhold
		limiter.setParameterT(1, 50.); // dynamics::limiter::Attack
		limiter.setParameterT(2, 50.); // dynamics::limiter::Release
		limiter.setParameterT(3, 1.);  // dynamics::limiter::Ratio
		limiter.setParameterT(4, 0.);  // dynamics::limiter::Sidechain
		
		this->setParameterT(0, 18.);
		this->setParameterT(1, 0.);
		this->setParameterT(2, 150.);
		this->setParameterT(3, 12.);
		this->setParameterT(4, 10.);
		this->setParameterT(5, 48.4);
		this->setParameterT(6, 25.);
		this->setParameterT(7, 1.);
		this->setParameterT(8, 0.68);
		this->setParameterT(9, 0.);
		this->setParameterT(10, 0.);
		this->setParameterT(11, 0.2);
		this->setParameterT(12, 11.);
		this->setParameterT(13, 0.);
		this->setParameterT(14, 0.);
		this->setParameterT(15, 0.);
		this->setExternalData({}, -1);
	}
	~instance() override
	{
		// Cleanup external data references --------------------------------------------------------
		
		this->setExternalData({}, -1);
	}
	
	static constexpr bool isPolyphonic() { return NV > 1; };
	
	static constexpr bool isProcessingHiseEvent() { return true; };
	
	static constexpr bool hasTail() { return true; };
	
	static constexpr bool isSuspendedOnSilence() { return false; };
	
	void setExternalData(const ExternalData& b, int index)
	{
		// External Data Connections ---------------------------------------------------------------
		
		this->getT(1).getT(0).getT(0).setExternalData(b, index);                                 // mod_fx_impl::clock_ramp_t<NV>
		this->getT(1).getT(1).setExternalData(b, index);                                         // mod_fx_impl::cable_pack_t<NV>
		this->getT(2).setExternalData(b, index);                                                 // mod_fx_impl::pack2_writer_t
		this->getT(3).getT(1).getT(1).getT(3).setExternalData(b, index);                         // mod_fx_impl::cable_table_t<NV>
		this->getT(4).getT(1).getT(1).getT(0).getT(5).getT(1).getT(0).setExternalData(b, index); // mod_fx_impl::simple_ar_t<NV>
		this->getT(4).getT(1).getT(1).getT(0).getT(6).setExternalData(b, index);                 // mod_fx_impl::clock_ramp1_t<NV>
		this->getT(4).getT(1).getT(1).getT(1).getT(5).getT(1).getT(0).setExternalData(b, index); // mod_fx_impl::simple_ar1_t<NV>
		this->getT(5).setExternalData(b, index);                                                 // mod_fx_impl::limiter_t
	}
};
}

#undef getT
#undef connectT
#undef setParameterT
#undef setParameterWT
#undef getParameterT
// ======================================| Public Definition |======================================

namespace project
{
// polyphonic template declaration

template <int NV>
using mod_fx = wrap::node<mod_fx_impl::instance<NV>>;
}


