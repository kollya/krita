/*
 *  Copyright (c) 2002 Patrick Julein <freak@codepimps.org>
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
 *  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */
#include <qhbox.h>
#include <qlabel.h>
#include <qlayout.h>
#include <kinstance.h>
#include <klocale.h>
#include <kstandarddirs.h>
#include <koIconChooser.h>
#include "integerwidget.h"
#include "kis_itemchooser.h"

KisItemChooser::KisItemChooser(const vKoIconItem& items, bool spacing, QWidget *parent, const char *name) : super(parent, name)
{
	m_doSpacing = spacing;

	if (spacing) {
		m_lbSpacing = new QLabel(i18n("Spacing: "), this);
		m_slSpacing = new IntegerWidget( 1, 100, this, "int_widget" );
		m_slSpacing -> setTickmarks(QSlider::Below);
		m_slSpacing -> setTickInterval(10);
		QObject::connect(m_slSpacing, SIGNAL(valueChanged(int)), this, SLOT(slotSetItemSpacing(int)));
	} else {
		m_lbSpacing = 0;
		m_slSpacing = 0;
	}

    	m_frame = new QHBox(this);
	m_frame -> setFrameStyle(QFrame::Panel | QFrame::Sunken);
	m_chooser = new KoIconChooser(QSize(30,30), m_frame, "icon_chooser");
	QObject::connect(m_chooser, SIGNAL(selected(KoIconItem*)), this, SLOT(slotItemSelected(KoIconItem*)));
	initGUI(spacing);

	QPtrListIterator<KoIconItem> itr(items);

	for (itr.toFirst(); itr.current(); ++itr)
		m_chooser -> addItem(itr.current());
}

KisItemChooser::KisItemChooser(bool spacing, QWidget *parent, const char *name)
{
	m_doSpacing = spacing;

	if (spacing) {
		m_lbSpacing = new QLabel(i18n("Spacing: "), this);
		m_slSpacing = new IntegerWidget( 1, 100, this, "int_widget" );
		m_slSpacing -> setTickmarks(QSlider::Below);
		m_slSpacing -> setTickInterval(10);
		QObject::connect(m_slSpacing, SIGNAL(valueChanged(int)), this, SLOT(slotSetItemSpacing(int)));
	} else {
		m_lbSpacing = 0;
		m_slSpacing = 0;
	}

	m_frame = new QHBox(this);
	m_frame -> setFrameStyle(QFrame::Panel | QFrame::Sunken);
	m_chooser = new KoIconChooser(QSize(30,30), m_frame, "icon_chooser");
	QObject::connect(m_chooser, SIGNAL(selected(KoIconItem*)), this, SLOT(slotItemSelected(KoIconItem*)));
	initGUI(spacing);
}

KisItemChooser::~KisItemChooser()
{
}

void KisItemChooser::initGUI(bool spacing)
{
	QVBoxLayout *mainLayout = new QVBoxLayout(this, 2, -1, "main layout");

	mainLayout -> addWidget(m_frame, 10);

	if (spacing) {
		QHBoxLayout *spacingLayout = new QHBoxLayout( -1, "spacing layout" );

		mainLayout -> addLayout(spacingLayout, 1);
		spacingLayout -> addWidget(m_lbSpacing, 0);
		spacingLayout -> addStretch();
		spacingLayout -> addWidget(m_slSpacing, 1);
	}
}

void KisItemChooser::setCurrent(KoIconItem *item)
{
	m_chooser -> setCurrentItem(item);

	if (m_doSpacing)
		m_slSpacing -> setValue(item -> spacing());
}

KoIconItem* KisItemChooser::currentItem()
{
	return m_chooser -> currentItem();
}

void KisItemChooser::slotItemSelected(KoIconItem *item)
{
	if (m_doSpacing && item)
		m_slSpacing -> setValue(item -> spacing());

	emit selected(item);
}

void KisItemChooser::slotSetItemSpacing(int spacingValue)
{
	KoIconItem *item = currentItem();

	if (m_doSpacing && item)
		item -> setSpacing(spacingValue);
}

void KisItemChooser::addItem(KoIconItem *item)
{
	m_chooser -> addItem(item);
}

void KisItemChooser::addItem(const vKoIconItem& items)
{
	QPtrListIterator<KoIconItem> itr(items);

	for (itr.toFirst(); itr.current(); ++itr)
		m_chooser -> addItem(itr.current());
}

#include "kis_itemchooser.moc"
