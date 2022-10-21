/*
 *  SPDX-FileCopyrightText: 2022 Dmitry Kazakov <dimula73@gmail.com>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 */
#ifndef KISPAINTTHICKNESSOPTIONWIDGET_H
#define KISPAINTTHICKNESSOPTIONWIDGET_H

#include <KisCurveOptionWidget2.h>
#include <KisPaintThicknessOptionData.h>


class KisPaintThicknessOptionWidget : public KisCurveOptionWidget2
{
    Q_OBJECT
public:
    using data_type = KisPaintThicknessOptionData;

    KisPaintThicknessOptionWidget(lager::cursor<KisPaintThicknessOptionData> optionData, lager::reader<bool> lightnessModeEnabled);
    ~KisPaintThicknessOptionWidget();

    void writeOptionSetting(KisPropertiesConfigurationSP setting) const;
    void readOptionSetting(const KisPropertiesConfigurationSP setting);

private:
    struct Private;
    const QScopedPointer<Private> m_d;
};

#endif // KISPAINTTHICKNESSOPTIONWIDGET_H
