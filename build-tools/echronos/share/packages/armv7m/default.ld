/*
 * eChronos Real-Time Operating System
 * Copyright (C) 2015  National ICT Australia Limited (NICTA), ABN 62 102 206 173.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, version 3, provided that these additional
 * terms apply under section 7:
 *
 *   No right, title or interest in or to any trade mark, service mark, logo
 *   or trade name of of National ICT Australia Limited, ABN 62 102 206 173
 *   ("NICTA") or its licensors is granted. Modified versions of the Program
 *   must be plainly marked as such, and must not be distributed using
 *   "eChronos" as a trade mark or product name, or misrepresented as being
 *   the original Program.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

OUTPUT_FORMAT("elf32-littlearm", "elf32-bigarm", "elf32-littlearm")
OUTPUT_ARCH(arm)
ENTRY(vector_table)
SECTIONS
{
        . = {{code_addr}};
        ro_start = .;
        .vectors : AT ({{flash_load_addr}})
        { *(.vectors) }

        .text : AT (LOADADDR(.vectors) + SIZEOF(.vectors))
        {
            *(.text.startup)
             *(.text)
        }

        . = LOADADDR (.text) + SIZEOF(.text);
        . = ALIGN(4);
        ro_data_start = .;

        .rodata : AT (ro_data_start)
        {
            *(.rodata*)
        }

        ro_end = ALIGN(4);

        . = {{data_addr}};
        .data.bitband : AT ({{flash_load_addr}} + (ro_end - ro_start))
        {
              *(.data.bitband)
        }

        /* Check that bitband variables don't overflow */
        dummy = ASSERT(. <= {{data_addr}} + {{bitband_size}}, "bitband variables too large");

        .data : AT (LOADADDR(.data.bitband) + SIZEOF(.data.bitband))
        {
            _init_array_start = .
            .init_array
             *(.data)
              . = ALIGN(4);
        }

        data_load_addr = LOADADDR(.data.bitband);
        data_virt_addr = ADDR(.data.bitband);
        data_size = . - ADDR(.data.bitband);

        /* Check addresses are correct */
        dummy = ASSERT(data_load_addr % 4 == 0, "data_load_addr not word aligned");
        dummy = ASSERT(data_virt_addr % 4 == 0, "data_virt_addr not word aligned");
        dummy = ASSERT(data_size % 4 == 0, "data_size not word aligned");

        .bss :
        {
             *(.bss)
             *(COMMON)
             . = ALIGN(4);
        }

        bss_virt_addr = ADDR(.bss);
        bss_size = SIZEOF(.bss);

        .stack : {
               . = . + {{stack_size}};
               stack = .;
        }

        /* Aliases for all the bitband variables */
        {{#bit_aliases}}
        {{.}}_bitband = {{bitband_alias}} + (({{.}} - {{bitband_base}}) * 32);
        {{/bit_aliases}}

        /* Debug sections */

        .debug_aranges  0 : { *(.debug_aranges) }
        .debug_pubnames 0 : { *(.debug_pubnames) }

        .debug_info     0 : { *(.debug_info .gnu.linkonce.wi.*) }
        .debug_abbrev   0 : { *(.debug_abbrev) }
        .debug_line     0 : { *(.debug_line) }
        .debug_frame    0 : { *(.debug_frame) }
        .debug_str      0 : { *(.debug_str) }
        .debug_loc      0 : { *(.debug_loc) }
        .debug_macinfo  0 : { *(.debug_macinfo) }

        .debug_pubtypes 0 : { *(.debug_pubtypes) }
        .debug_ranges   0 : { *(.debug_ranges) }

        .debug_macro    0 : { *(.debug_macro) }

        /* Include the list of initialization functions sorted. */
       /* .init_array :
        {
            *(.init_array)
            KEEP (*(SORT(EXCLUDE_FILE(crti.opp crtn.opp) .init_array.*)))
            KEEP (*(EXCLUDE_FILE(crti.opp crtn.opp) .init_array))
            *(.init_array)
        }*/

        /* Include the list of termination functions sorted. */
        /*.fini_array :
        {
            *(.fini_array)
            KEEP (*(SORT(EXCLUDE_FILE(crti.opp crtn.opp) .fini_array.*)))
            KEEP (*(EXCLUDE_FILE(crti.opp crtn.opp) .fini_array))
            *(.fini_array)
        }*/

        /DISCARD/ : { *(*) }
}
