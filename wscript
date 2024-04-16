#!/usr/bin/env python3
# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (c) 2012-2020 Association Prologin <association@prologin.org>

import glob
import os
import os.path
import shutil

from wafgenerator import generator_player_install



def options(opt):
    ...


def configure(cfg):
    cfg.load('compiler_cxx')
    cfg.env.append_value('INCLUDES', ['lib', 'src'])


def _copy_assets(source_dir, build_dir):
    assets = [
        'maps/colibri.json',
    ]
    for asset in assets:
        print(os.path)
        src_path = os.path.join(source_dir, asset)
        dst_path = os.path.join(build_dir, os.path.basename(asset))
        try:
            shutil.copyfile(src_path, dst_path)
        except IOError as exception:
            print(exception)


def build(bld):
    bld.shlib(
        source='''
            src/action_tourner_case.cc
            src/action_activer_aigle.cc
            src/action_deplacer_aigle.cc
            src/action_debug_poser_drakkar.cc
            src/api.cc
            src/entry.cc
            src/game_state.cc
            src/interface.cc
            src/rules.cc
            src/carte.cc
        ''',
        defines=['MODULE_COLOR=ANSI_COL_BROWN', 'MODULE_NAME="rules"'],
        target='prologin2024',
        use=['stechec2'],
    )
    if not os.environ.get("WSCRIPT_SKIP_TESTS"):
        source_dir = bld.path.abspath()
        build_dir = os.path.join(bld.out_dir, os.path.relpath(source_dir, bld.top_dir))
        os.makedirs(build_dir, exist_ok=True)
        _copy_assets(source_dir, build_dir)


    abs_pattern = os.path.join(bld.path.abspath(), 'src/tests/test-*.cc')
    for test_src in glob.glob(abs_pattern):

        test_name = os.path.split(test_src)[-1]
        test_name = test_name[len("test-"):-(len(".cc"))]

        # Waf requires a relative path for the source
        src_relpath = os.path.relpath(test_src, bld.path.abspath())

        bld.program(
            features='gtest',
            source=src_relpath,
            target='prologin2024-test-{}'.format(test_name),
            use=['prologin2024', 'stechec2-utils'],
            includes=['.'],
            defines=['MODULE_COLOR=ANSI_COL_PURPLE',
                     'MODULE_NAME="prologin2024"'],
        )

    bld.install_files('${PREFIX}/share/stechec2/prologin2024', [
        'prologin2024.yml',
    ])

    generator_player_install(bld, 'prologin2024')
