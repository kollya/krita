/*
 *  Copyright (c) 2010 Lukáš Tvrdý <lukast.dev@gmail.com>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#include "kis_particle_paintop.h"
#include "kis_particle_paintop_settings.h"

#include <cmath>

#include "kis_vec.h"

#include <KoCompositeOp.h>

#include <kis_image.h>
#include <kis_debug.h>

#include <kis_global.h>
#include <kis_paint_device.h>
#include <kis_painter.h>
#include <kis_lod_transform.h>
#include <kis_types.h>
#include <kis_paintop_plugin_utils.h>
#include <brushengine/kis_paintop.h>
#include <brushengine/kis_paint_information.h>

#include "kis_particleop_option.h"

#include "particle_brush.h"

KisParticlePaintOp::KisParticlePaintOp(const KisPaintOpSettingsSP settings, KisPainter * painter, KisNodeSP node, KisImageSP image)
    : KisPaintOp(painter)
    , m_settings(settings)
{
    Q_UNUSED(image);
    Q_UNUSED(node);

    m_properties.particleCount = settings->getInt(PARTICLE_COUNT);
    m_properties.iterations = settings->getInt(PARTICLE_ITERATIONS);
    m_properties.gravity = settings->getDouble(PARTICLE_GRAVITY);
    m_properties.weight = settings->getDouble(PARTICLE_WEIGHT);
    m_properties.scale = QPointF(settings->getDouble(PARTICLE_SCALE_X), settings->getDouble(PARTICLE_SCALE_Y));

    m_particleBrush.setProperties(&m_properties);
    m_particleBrush.initParticles();

    m_rateOption.readOptionSetting(settings);
    m_rateOption.resetAllSensors();

    m_first = true;
}

KisParticlePaintOp::~KisParticlePaintOp()
{
}

KisSpacingInformation KisParticlePaintOp::paintAt(const KisPaintInformation& info)
{
    doPaintLine(info, info);
    return KisPaintOpPluginUtils::effectiveSpacing(0.0, 0.0, true, 0.0, false, 0.0, false, 0.0,
                                                   KisLodTransform::lodToScale(painter()->device()),
                                                   m_settings, nullptr, &m_rateOption, info);
}

void KisParticlePaintOp::paintLine(const KisPaintInformation &pi1, const KisPaintInformation &pi2,
                                   KisDistanceInformation *currentDistance)
{
    // Use superclass behavior for lines of zero length. Otherwise, airbrushing can happen faster
    // than it is supposed to.
    if (pi1.pos() == pi2.pos()) {
        KisPaintOp::paintLine(pi1, pi2, currentDistance);
    } else {
        doPaintLine(pi1, pi2);
    }
}

void KisParticlePaintOp::doPaintLine(const KisPaintInformation &pi1, const KisPaintInformation &pi2)
{
    if (!painter()) return;

    if (!m_dab) {
        m_dab = source()->createCompositionSourceDevice();
    }
    else {
        m_dab->clear();
    }


    if (m_first) {
        m_particleBrush.setInitialPosition(pi1.pos());
        m_first = false;
    }

    m_particleBrush.draw(m_dab, painter()->paintColor(), pi2.pos());
    QRect rc = m_dab->extent();

    painter()->bitBlt(rc.x(), rc.y(), m_dab, rc.x(), rc.y(), rc.width(), rc.height());
    painter()->renderMirrorMask(rc, m_dab);
}
