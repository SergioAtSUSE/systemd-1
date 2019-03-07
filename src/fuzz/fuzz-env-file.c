/* SPDX-License-Identifier: LGPL-2.1+ */

#include <errno.h>

#include "alloc-util.h"
#include "env-file.h"
#include "fd-util.h"
#include "fuzz.h"
#include "strv.h"

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
        _cleanup_fclose_ FILE *f = NULL;
        _cleanup_strv_free_ char **rl = NULL, **rlp =  NULL;

        if (size == 0)
                return 0;

        f = fmemopen((char*) data, size, "re");
        assert_se(f);

        /* We don't want to fill the logs with messages about parse errors.
         * Disable most logging if not running standalone */
        if (!getenv("SYSTEMD_LOG_LEVEL"))
                log_set_max_level(LOG_CRIT);

        (void) load_env_file(f, NULL, &rl);
        assert_se(fseek(f, 0, SEEK_SET) == 0);
        (void) load_env_file_pairs(f, NULL, &rlp);

        return 0;
}