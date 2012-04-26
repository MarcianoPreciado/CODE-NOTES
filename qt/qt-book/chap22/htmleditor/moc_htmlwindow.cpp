/****************************************************************************
** Meta object code from reading C++ file 'htmlwindow.h'
**
** Created: Wed Apr 25 14:42:15 2012
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "htmlwindow.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'htmlwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_HtmlWindow[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      12,   11,   11,   11, 0x08,
      24,   11,   19,   11, 0x08,
      31,   11,   19,   11, 0x08,
      40,   11,   11,   11, 0x08,
      64,   11,   11,   11, 0x08,
      72,   11,   11,   11, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_HtmlWindow[] = {
    "HtmlWindow\0\0open()\0bool\0save()\0saveAs()\0"
    "scriptActionTriggered()\0about()\0"
    "documentWasModified()\0"
};

void HtmlWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        HtmlWindow *_t = static_cast<HtmlWindow *>(_o);
        switch (_id) {
        case 0: _t->open(); break;
        case 1: { bool _r = _t->save();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 2: { bool _r = _t->saveAs();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 3: _t->scriptActionTriggered(); break;
        case 4: _t->about(); break;
        case 5: _t->documentWasModified(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData HtmlWindow::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject HtmlWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_HtmlWindow,
      qt_meta_data_HtmlWindow, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &HtmlWindow::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *HtmlWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *HtmlWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_HtmlWindow))
        return static_cast<void*>(const_cast< HtmlWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int HtmlWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
