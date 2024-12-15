/* See LICENSE for license details. */
#define _XOPEN_SOURCE 700
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <ddcutil_c_api.h>
#include <ddcutil_macros.h>
#include <ddcutil_status_codes.h>
#include <ddcutil_types.h>

#define BRIGHTNESS_FEATURE_CODE 0x10

#define DDC_DIE(e) \
    fprintf(stderr, \
            "(%s:%s:%d) returned %d (%s): %s\n", \
            __FILE__, \
            __func__, \
            __LINE__, \
            e, \
            ddca_rc_name(e), \
            ddca_rc_desc(e)); \
    exit(EXIT_FAILURE)

DDCA_Status set_continuous_value(
    DDCA_Display_Handle dh,
    const DDCA_Vcp_Feature_Code feature_code,
    const uint16_t value)
{
    return ddca_set_non_table_vcp_value(dh, feature_code, (value >> 8) & 0xFF, value & 0x00FF);
}

int main(int argc, char *argv[])
{
    if (argc <= 1) {
        fprintf(stderr,
                "# Set monitor 1 to 100%% brightness\n"
                "mmbctl 100\n"
                "\n"
                "# Set monitor 2 to 0%% brightness (lowest brightness, not completely off),\n"
                "# but leave monitor 1 untouched\n"
                "mmbctl -1 0\n"
                "\n"
                "# Set monitor 1 to 50%% brightness, skip monitor 2, and set monitor 3 to max brightness\n"
                "mmbctl 50 -1 100\n");
        exit(EXIT_FAILURE);
    }

    DDCA_Status err = ddca_init2("", DDCA_SYSLOG_NOT_SET, DDCA_INIT_OPTIONS_NONE, NULL);
    if (err != DDCRC_OK) {
        DDC_DIE(err);
    }

    DDCA_Display_Info_List *dlist = NULL;
    if ((err = ddca_get_display_info_list2(false, &dlist)) != DDCRC_OK || !dlist) {
        DDC_DIE(err);
    }

    for (int i = 0; i < dlist->ct; i++) {
        if (i + 1 == argc) {
            break;
        }

        const long brightness_value = strtol(argv[i + 1], NULL, 10);
        if (brightness_value == -1) {
            fprintf(stderr, "Encountered -1 brightness for monitor %d, skipping.\n", i);
            continue;
        } else if (brightness_value > 100 || brightness_value < -1) {
            fprintf(stderr,
                    "Error: valid brightness values must be in range [-1, 100], supplied %ld for monitor %d\n",
                    brightness_value,
                    i + 1);
            exit(EXIT_FAILURE);
        }

        DDCA_Display_Handle dh = NULL;
        if ((err = ddca_open_display2(dlist->info[i].dref, false, &dh)) != DDCRC_OK || !dh) {
            fprintf(stderr,
                    "Unable to connect to monitor (%s %s): %s\n",
                    dlist->info[i].model_name,
                    dlist->info[i].mfg_id,
                    ddca_rc_desc(err));
            continue;
        }

        if ((err = set_continuous_value(dh, BRIGHTNESS_FEATURE_CODE, (uint16_t)brightness_value)) != DDCRC_OK) {
            ddca_close_display(dh);
            ddca_free_display_info_list(dlist);
            DDC_DIE(err);
        }
        ddca_close_display(dh);
    }

    ddca_free_display_info_list(dlist);

    fprintf(stderr, "Done.\n");

    return EXIT_SUCCESS;
}
