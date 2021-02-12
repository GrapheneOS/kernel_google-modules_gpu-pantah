/*
 *
 * (C) COPYRIGHT ARM Limited. All rights reserved.
 *
 * This program is free software and is provided to you under the terms of the
 * GNU General Public License version 2 as published by the Free Software
 * Foundation, and any use by you of this program is subject to the terms
 * of such GNU licence.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, you can access it online at
 * http://www.gnu.org/licenses/gpl-2.0.html.
 *
 * SPDX-License-Identifier: GPL-2.0
 *
 *//* SPDX-License-Identifier: GPL-2.0 */
/*
 *
 * (C) COPYRIGHT 2019-2020 ARM Limited. All rights reserved.
 *
 * This program is free software and is provided to you under the terms of the
 * GNU General Public License version 2 as published by the Free Software
 * Foundation, and any use by you of this program is subject to the terms
 * of such GNU license.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, you can access it online at
 * http://www.gnu.org/licenses/gpl-2.0.html.
 *
 */

#if !defined(_KBASE_TIMELINE_PRIV_H)
#define _KBASE_TIMELINE_PRIV_H

#include <mali_kbase.h>
#include "mali_kbase_tlstream.h"

#if MALI_USE_CSF
#include "csf/mali_kbase_csf_tl_reader.h"
#include "csf/mali_kbase_csf_trace_buffer.h"
#endif

#include <linux/timer.h>
#include <linux/atomic.h>
#include <linux/mutex.h>

/**
 * struct kbase_timeline - timeline state structure
 * @streams:                The timeline streams generated by kernel
 * @tl_kctx_list:           List of contexts for timeline.
 * @tl_kctx_list_lock:      Lock to protect @tl_kctx_list.
 * @autoflush_timer:        Autoflush timer
 * @autoflush_timer_active: If non-zero autoflush timer is active
 * @reader_lock:            Reader lock. Only one reader is allowed to
 *                          have access to the timeline streams at any given time.
 * @event_queue:            Timeline stream event queue
 * @bytes_collected:        Number of bytes read by user
 * @timeline_flags:         Zero, if timeline is disabled. Timeline stream flags
 *                          otherwise. See kbase_timeline_io_acquire().
 * @obj_header_btc:         Remaining bytes to copy for the object stream header
 * @aux_header_btc:         Remaining bytes to copy for the aux stream header
 */
struct kbase_timeline {
	struct kbase_tlstream streams[TL_STREAM_TYPE_COUNT];
	struct list_head  tl_kctx_list;
	struct mutex      tl_kctx_list_lock;
	struct timer_list autoflush_timer;
	atomic_t          autoflush_timer_active;
	struct mutex      reader_lock;
	wait_queue_head_t event_queue;
#if MALI_UNIT_TEST
	atomic_t          bytes_collected;
#endif /* MALI_UNIT_TEST */
	atomic_t         *timeline_flags;
	size_t            obj_header_btc;
	size_t            aux_header_btc;
#if MALI_USE_CSF
	struct kbase_csf_tl_reader csf_tl_reader;
#endif
};

extern const struct file_operations kbasep_tlstream_fops;

void kbase_create_timeline_objects(struct kbase_device *kbdev);

#endif /* _KBASE_TIMELINE_PRIV_H */
