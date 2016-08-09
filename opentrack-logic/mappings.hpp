/* Copyright (c) 2014-2015 Stanislaw Halik
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 */

#pragma once

#include <QSettings>
#include "opentrack-compat/options.hpp"
using namespace options;
#include "spline-widget/functionconfig.h"
#include "main-settings.hpp"

class Mapping {
public:
    Mapping(QString primary,
            QString secondary,
            int max_x,
            int max_y,
            axis_opts& opts) :
        curve(max_x, max_y),
        curveAlt(max_x, max_y),
        opts(opts),
        name1(primary),
        name2(secondary)
    {
        mem<QSettings> iniFile = group::ini_file();
        curve.loadSettings(*iniFile, primary);
        curveAlt.loadSettings(*iniFile, secondary);
    }
    Map curve;
    Map curveAlt;
    axis_opts& opts;
    QString name1, name2;
};

class Mappings {
private:
    Mapping axes[6];
public:
    Mappings(std::vector<axis_opts*> opts) :
        axes {
            Mapping("tx","tx_alt", 30, 50, *opts[TX]),
            Mapping("ty","ty_alt", 30, 50, *opts[TY]),
            Mapping("tz","tz_alt", 30, 50, *opts[TZ]),
            Mapping("rx", "rx_alt", 180, 180, *opts[Yaw]),
            Mapping("ry", "ry_alt", 180, 180, *opts[Pitch]),
            Mapping("rz", "rz_alt", 180, 180, *opts[Roll])
        }
    {}

    inline Mapping& operator()(int i) { return axes[i]; }
    inline const Mapping& operator()(int i) const { return axes[i]; }


    inline Mapping& operator()(unsigned i) { return axes[i]; }
    inline const Mapping& operator()(unsigned i) const { return axes[i]; }

    void load_mappings()
    {
        mem<QSettings> iniFile = group::ini_file();

        for (int i = 0; i < 6; i++)
        {
            axes[i].curve.loadSettings(*iniFile, axes[i].name1);
            axes[i].curveAlt.loadSettings(*iniFile, axes[i].name2);
            axes[i].opts.b->reload();
        }
    }
    void save_mappings()
    {
        mem<QSettings> iniFile = group::ini_file();

        for (int i = 0; i < 6; i++)
        {
            axes[i].curve.saveSettings(*iniFile, axes[i].name1);
            axes[i].curveAlt.saveSettings(*iniFile, axes[i].name2);
            axes[i].opts.b->save();
        }
    }

    void invalidate_unsaved()
    {
        for (int i = 0; i < 6; i++)
        {
            axes[i].curve.invalidate_unsaved_settings();
            axes[i].curveAlt.invalidate_unsaved_settings();
            axes[i].opts.b->reload();
        }
    }
};
