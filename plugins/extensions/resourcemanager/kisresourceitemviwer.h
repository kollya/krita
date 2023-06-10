#ifndef KISRESOURCEITEMVIWER_H
#define KISRESOURCEITEMVIWER_H

#include <QWidget>

#include "KisPopupButton.h"
#include "ResourceListViewModes.h"


enum class Viewer {TableAvailable, ResourceManager, TableSelected};

namespace Ui {
class KisResourceItemViwer;
}

class KisResourceItemViwer : public KisPopupButton
{
    Q_OBJECT

public:
    explicit KisResourceItemViwer(Viewer type, QWidget *parent = nullptr);
    ~KisResourceItemViwer();

    void updateViewSettings();

private Q_SLOTS:
    void slotViewThumbnail();
    void slotViewDetails();

Q_SIGNALS:
    void onViewThumbnail();
    void onViewDetails();

private:
    Ui::KisResourceItemViwer *m_ui;

    ListViewMode m_mode;
    Viewer m_type;

};

#endif // KISRESOURCEITEMVIWER_H
