// @(#)root/qt:$Name:  $:$Id: TQtWidget.h,v 1.7 2004/08/02 08:14:43 rdm Exp $
// Author: Valeri Fine   21/01/2002

/*************************************************************************
 * Copyright (C) 1995-2004, Rene Brun and Fons Rademakers.               *
 * Copyright (C) 2002 by Valeri Fine.                                    *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

#ifndef ROOT_TQtWidget
#define ROOT_TQtWidget

// Definition of TQtWidget class
// "double-buffered" widget

#include <assert.h>

#include <qwidget.h>
#include <qpixmap.h>

class TCanvas;

//___________________________________________________________________
class TQtWidgetBuffer : public QPixmap
{
  private:
    QWidget *fWidget;

  public:
    TQtWidgetBuffer(QWidget *w=0) :  QPixmap(), fWidget(w)
    { if (w) resize(w->size()); }
    inline QRect rect () const { return fWidget->rect();}
};
//___________________________________________________________________
class  TQtWidget : public QWidget {
 Q_OBJECT
public:
   enum {
      kEXITSIZEMOVE,
      kENTERSIZEMOVE,
      kFORCESIZE
   };
  TQtWidget( QWidget* parent=0, const char* name=0, WFlags f=Qt::WStyle_NoBorder, bool embedded=TRUE);
  virtual ~TQtWidget();
  void SetCanvas(TCanvas *c)                 { fCanvas = c;}
  inline TCanvas  *GetCanvas() const         { return fCanvas;}
  inline QPixmap  &GetBuffer()               { return fPixmapID;}
  inline const QPixmap  &GetBuffer()  const  { return fPixmapID;}

  // overloaded methods
  virtual void adjustSize();
  virtual void resize (int w, int h);
  virtual void erase ();
  bool    IsDoubleBuffered() { return fDoubleBufferOn; }
  void    SetDoubleBuffer(bool on=TRUE){ fDoubleBufferOn = on;}
  virtual void SetSaveFormat(const char *format);

protected:
   friend class TGQt;
   TCanvas         *fCanvas;
   TQtWidgetBuffer  fPixmapID; // Double buffer of this widget
   bool        fPaint;
   bool        fSizeChanged;
   bool        fDoubleBufferOn;
   bool        fEmbedded;
   QSize       fSizeHint;
   QWidget    *fWrapper;
   QString     fSaveFormat;
   void SetRootID(QWidget *wrapper);
   QWidget *GetRootID() const;
   virtual void EmitCanvasPainted() { emit CanvasPainted(); }
   TCanvas  *Canvas();
   bool paintFlag(bool mode=TRUE);
   void AdjustBufferSize();

   // overloaded QWidget methods
   bool paintingActive () const;

   virtual void enterEvent       ( QEvent *      );
   virtual void customEvent      ( QCustomEvent *);
   virtual void focusInEvent     ( QFocusEvent * );
   virtual void focusOutEvent    ( QFocusEvent * );
   virtual void leaveEvent       ( QEvent *      );
   virtual void mouseDoubleClickEvent(QMouseEvent* );
   virtual void mouseMoveEvent   ( QMouseEvent * );
   virtual void mousePressEvent  ( QMouseEvent * );
   virtual void mouseReleaseEvent( QMouseEvent * );
   virtual void keyPressEvent    ( QKeyEvent *   );
   virtual void keyReleaseEvent  ( QKeyEvent *   );
   virtual void showEvent        ( QShowEvent *  );
   virtual void paintEvent       ( QPaintEvent * );
   virtual void resizeEvent      ( QResizeEvent *);
   //  Layout methods:
   virtual void        SetSizeHint (const QSize &size);
public:
   virtual QSize       sizeHint () const;        //  returns the preferred size of the widget.
   virtual QSize       minimumSizeHint () const; // returns the smallest size the widget can have.
   virtual QSizePolicy sizePolicy () const;      //  returns a QSizePolicy; a value describing the space requirements
protected:
   // -- A special event handler
   virtual void exitSizeEvent ();
   virtual void stretchWidget(QResizeEvent *e);

public slots:
   virtual void cd();
   virtual void cd(int subpadnumber);
   void Disconnect();
   void Refresh();
   virtual bool Save(const QString &fileName) const;
   virtual bool Save(const char *fileName) const;
   virtual bool Save(const QString &fileName,const char *format,int quality=60) const;
   virtual bool Save(const char *fileName,const char *format,int quality=60) const;
signals:
   // emit the Qt signal when the double buffer of the TCamvas has been filled up
   void CanvasPainted();  // Signal the TCanvas has been oainted ionto the screen
   void Saved(bool ok); // Signal the TCanvas has been saved into the file

};

//______________________________________________________________________________
inline void TQtWidget::AdjustBufferSize()
   {  if (fPixmapID.size() != size() ) fPixmapID.resize(size()); }

//______________________________________________________________________________
inline bool TQtWidget::paintingActive () const {
  return QWidget::paintingActive() || fPixmapID.paintingActive();
}
//______________________________________________________________________________
inline void TQtWidget::SetRootID(QWidget *wrapper) { fWrapper = wrapper;}
//______________________________________________________________________________
inline QWidget *TQtWidget::GetRootID() const { return fWrapper;}

#endif
