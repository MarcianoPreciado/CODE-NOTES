/****************************************************************************
** Meta object code from reading C++ file 'booleanwindow.h'
**
** Created: Sat May 19 15:22:18 2012
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "booleanwindow.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'booleanwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_BooleanWindow[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      20,   15,   14,   14, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_BooleanWindow[] = {
    "BooleanWindow\0\0expr\0"
    "booleanExpressionChanged(QString)\0"
};

void BooleanWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        BooleanWindow *_t = static_cast<BooleanWindow *>(_o);
        switch (_id) {
        case 0: _t->booleanExpressionChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData BooleanWindow::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject BooleanWindow::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_BooleanWindow,
      qt_meta_data_BooleanWindow, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &BooleanWindow::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *BooleanWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *BooleanWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_BooleanWindow))
        return static_cast<void*>(const_cast< BooleanWindow*>(this));
    return QWidget::qt_metacast(_clname);
}

int BooleanWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
