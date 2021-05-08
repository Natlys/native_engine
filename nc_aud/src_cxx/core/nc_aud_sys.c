#include "../nc_aud_pch.h"
#include "nc_aud_sys.h"
#if (defined NC_API)
// includes //
#   include "../../nc_lib/src_cxx/core/nc_lib_load.h"
#   include <mmsystem.h>
// types //
typedef HWAVEOUT               nc_aud_odvc_t;
typedef WAVEHDR            nc_aud_odvc_hdr_t;
typedef WAVEFORMATEX       nc_aud_odvc_fmt_t;
typedef WAVEOUTCAPS        nc_aud_odvc_cap_t;
// functions //
// // hardware // //
// // // general // // //
typedef v1u32_t (NC_API_ENTRYP nc_aud_odvc_new_ft) (
    nc_aud_odvc_t* dvc_handle, v1u32_t dvc_index, nc_aud_odvc_fmt_t* dvc_format,
    v1u64_t* callback, v1u64_t* user_data, v1u64_t open_flag
); nc_aud_odvc_new_ft nc_aud_odvc_new = NC_NULL;
typedef v1u_t (NC_API_ENTRYP nc_aud_odvc_del_ft) (
    nc_aud_odvc_t dvc_handle
); nc_aud_odvc_del_ft nc_aud_odvc_del = NC_NULL;
// // // getters // // //
typedef v1u32_t (NC_API_ENTRYP nc_aud_odvc_get_num_ft) (
    v1nil_t
); nc_aud_odvc_get_num_ft nc_aud_odvc_get_num = NC_NULL;
typedef v1u_t (NC_API_ENTRYP nc_aud_odvc_get_cap_ft) (
    v1u64_t dvc_index, nc_aud_odvc_cap_t* dvc_cap, v1u32_t nof_bytes
); nc_aud_odvc_get_cap_ft nc_aud_odvc_get_cap = NC_NULL;
// // // setters // // //
// // // commands // // //
// // native // //
typedef v1nil_t(NC_API_ENTRYP nc_aud_odvc_proc_ft) (
    nc_aud_odvc_t dvc, v1u32_t mesg,
    v1u64_t data, v1u64_t param0, v1u64_t param1
); // nc_aud_odvc_proc_ft nc_aud_odvc_proc = NC_NULL;
/// audio_system_type
/// description:
/// ->handles all the boilerplate for audio synthezink;
/// ->owns an audio device to make blip blop and beep;
/// interface:
/// ->get some info: device list...
/// ->set some stuff: device name, nof_samples...
/// ->initialize;
/// ->enjoy the sound;
typedef struct nc_aud_sys_t {
    // library //
    nc_lib_loader_t loader;
    // output_device //
    nc_aud_odvc_t odvc_handle;
    nc_aud_odvc_fmt_t odvc_format;
    nc_aud_odvc_hdr_t* odvc_headers;
    nc_aud_odvc_proc_ft odvc_proc;
    v1u_t odvc_indx;
    v1u_t odvc_numb;
    // config //
    data_t cell_data;
    v1u_t cell_numb;
    v1u_t cell_curr;
    v1u_t cell_free;
    v1u_t samp_numb;
    v1u_t samp_rate;
    v1u_t chnl_numb;
} nc_aud_sys_t;
typedef const nc_aud_sys_t nc_aud_sys_tc;
nc_aud_sys_t nc_aud_sys;
// getters //
// setters //
v1bit_t nc_aud_sys_set_samp_rate(v1u_t sample_rate) {
    // checking //
    NC_PCALL({
        NC_CHECK(sample_rate >= NC_AUD_MINIMAL_SAMP_RATE, "too few samples!", return NC_FALSE);
        NC_CHECK(sample_rate <= NC_AUD_MAXIMAL_SAMP_RATE, "too many samples!", return NC_FALSE);
    }, "setting error", return NC_FALSE);
    // action //
    nc_aud_sys.samp_rate = sample_rate;
    // result //
    return NC_TRUTH;
}
v1bit_t nc_aud_sys_set_chnl_numb(v1u_t nof_channels) {
    // checking //
    NC_PCALL({
        NC_CHECK(nof_channels >= NC_AUD_MINIMAL_CHNL_NUMB, "too few channels!", return NC_FALSE);
        NC_CHECK(nof_channels <= NC_AUD_MAXIMAL_CHNL_NUMB, "too many channels!", return NC_FALSE);
    }, "setting error", return NC_FALSE);
    // action //
    NC_PCALL({
        nc_aud_sys.chnl_numb = nof_channels;
    }, "setting error", return NC_FALSE);
    // result //
    return NC_TRUTH;
}
// predicates //
// commands //
static NC_API_ENTRY v1nil_t nc_aud_sys_odvc_proc(
    nc_aud_odvc_t hdvc, v1u32_t mesg, v1u64_t data, v1u64_t param0, v1u64_t param1
) {
    // checking //
	if (mesg != WOM_DONE) { return; }
	NC_PCALL({ // action //
        nc_aud_sys_t sys = *(nc_aud_sys_t*)data;
        sys.cell_free  += 1u;
    }, "work error!", return NC_FALSE);
    // result //
}
v1bit_t nc_aud_sys_init() {
    NC_PCALL({ // checking //
        //NC_CHECK(nc_aud_sys.on_beep != NC_NULL, "init error!", return NC_FALSE);
        //NC_CHECK(nc_aud_sys.nof_channels != NC_ZERO, "init error!", return NC_FALSE);
        //NC_CHECK(nc_aud_sys.sample_rate != NC_ZERO, "init error!", return NC_FALSE);
    }, "init error!", return NC_FALSE);
    NC_PCALL({ // action //
        NC_PCALL({ // common_setup //
            memset(&nc_aud_sys.odvc_format, NC_ZERO, sizeof(nc_aud_odvc_fmt_t));
            nc_aud_sys.odvc_handle = NC_NULL;
            nc_aud_sys.odvc_proc = nc_aud_sys_odvc_proc;
            nc_aud_sys.odvc_numb = NC_ZERO;
            nc_aud_sys.odvc_indx = NC_ZERO;
            nc_aud_sys.odvc_headers = NC_NULL;
            nc_aud_sys.samp_rate = NC_AUD_DEFAULT_SAMP_RATE;
            nc_aud_sys.samp_numb = NC_AUD_DEFAULT_SAMP_NUMB;
            nc_aud_sys.cell_data = NC_NULL;
            nc_aud_sys.cell_numb = NC_AUD_DEFAULT_CELL_NUMB;
            nc_aud_sys.cell_free = NC_ZERO;
            nc_aud_sys.chnl_numb = NC_UNIT;
        }, "init error!", return NC_FALSE);
        NC_PCALL({ // library //
            nc_lib_loader_ctor(&nc_aud_sys.loader);
            nc_lib_loader_set_name(&nc_aud_sys.loader, "winmm.dll");
            NC_CHECK(nc_lib_loader_init(&nc_aud_sys.loader), "init error!", return NC_FALSE);
            // general //
            NC_CHECK(nc_lib_loader_load(&nc_aud_sys.loader, "waveOutOpen", &nc_aud_odvc_new), "load error!", return NC_FALSE);
            NC_CHECK(nc_lib_loader_load(&nc_aud_sys.loader, "waveOutClose", &nc_aud_odvc_del), "load error!", return NC_FALSE);
            // getters //
            NC_CHECK(nc_lib_loader_load(&nc_aud_sys.loader, "waveOutGetNumDevs", &nc_aud_odvc_get_num), "load error!", return NC_FALSE);
            NC_CHECK(nc_lib_loader_load(&nc_aud_sys.loader, "waveOutGetDevCapsA", &nc_aud_odvc_get_cap), "load error!", return NC_FALSE);
            // setters //
            // commands //
        }, "init error!", return NC_FALSE);
        NC_PCALL({ // hardware //
            nc_aud_sys.odvc_numb = nc_aud_odvc_get_num();
            WAVEOUTCAPS wave_cap;
            for (v1u_t itr = 0u; itr < nc_aud_sys.odvc_numb; itr++) {
                if (nc_aud_odvc_get_cap(itr, &wave_cap, sizeof(WAVEOUTCAPS)) == S_OK){
                    NC_OLOG(
                        "wave_out_cap is found:" "{" NC_STR_EOL
                            "name: %s;" NC_STR_EOL
                            "channels: %d;" NC_STR_EOL
                            "version: %d;" NC_STR_EOL
                        "};",
                        wave_cap.szPname,
                        wave_cap.wChannels,
                        wave_cap.vDriverVersion
                    );
                    system("pause");
                }
            }
            nc_aud_sys.odvc_format.cbSize = sizeof(nc_aud_odvc_fmt_t);
            nc_aud_sys.odvc_format.wFormatTag = WAVE_FORMAT_PCM;
            nc_aud_sys.odvc_format.nChannels = nc_aud_sys.chnl_numb;
            nc_aud_sys.odvc_format.wBitsPerSample = sizeof(byte_t) * 8ul;
            nc_aud_sys.odvc_format.nBlockAlign = sizeof(byte_t) * nc_aud_sys.chnl_numb;
            nc_aud_sys.odvc_format.nSamplesPerSec = nc_aud_sys.sample_rate;
            nc_aud_sys.odvc_format.nAvgBytesPerSec = sizeof(byte_t) * nc_aud_sys.sample_rate * nc_aud_sys.nof_channels;
            NC_CHECK(
                nc_aud_odvc_new(
                    nc_aud_sys.odvc_handle, nc_aud_sys.odvc_indx, &nc_aud_sys.odvc_format, nc_aud_sys.odvc_proc,
                    &nc_aud_sys, CALLBACK_FUNCTION
                ) != S_OK, "init error!", return NC_FALSE);
        }, "init error!", return NC_FALSE);
    }, "init error!", return NC_FALSE);
    // logging //
    NC_OLOG(
        "audio_system_init:" "{" NC_STR_EOL
        "   device:" NC_STR_EOL
        "   sample_rate:" NC_STR_EOL
        "   nof_channels:" NC_STR_EOL
        "};"
    );
    NC_PCALL(return NC_FALSE, "quit error", return NC_FALSE);
    // result //
    return NC_TRUTH;
}
v1bit_t nc_aud_sys_quit() {
    // logging //
    // checking //
    NC_PCALL({
        //NC_CHECK(nc_aud_sys.on_beep != NC_NULL, "quit error!", return NC_FALSE);
        //NC_CHECK(nc_aud_sys.nof_channels != NC_ZERO, "quit error!", return NC_FALSE);
        //NC_CHECK(nc_aud_sys.sample_rate != NC_ZERO, "quit error!", return NC_FALSE);
    }, "quit error!", return NC_FALSE);
    NC_PCALL({ // action //
        NC_PCALL({ // hardware //
            nc_aud_odvc_del(nc_aud_sys.odvc_handle);
            nc_aud_sys.odvc_handle = NC_NULL;
        }, "quit error", return NC_FALSE);
        NC_PCALL({ // library //
            NC_CHECK(nc_lib_loader_quit(&nc_aud_sys.loader), "init error!", return NC_FALSE);
            nc_lib_loader_dtor(&nc_aud_sys.loader);
        }, "quit error", return NC_FALSE);
    }, "quit error!", return NC_FALSE);
    // result //
    return NC_TRUTH;
}
v1bit_t nc_aud_sys_work() {
    // checking //
    // logging //
    // result //
    return NC_TRUTH;
}
// end_of_file //
#endif  // NC_API //