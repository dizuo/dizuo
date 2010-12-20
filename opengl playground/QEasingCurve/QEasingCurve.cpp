#include "qeasingcurve.h"
#include <cassert>

static bool isConfigFunction(QEasingCurve::Type type)
{
    return type >= QEasingCurve::InElastic
            && type <= QEasingCurve::OutInBounce;
}

class QEasingCurveFunction
{
public:
    enum Type { In, Out, InOut, OutIn };

    QEasingCurveFunction(QEasingCurveFunction::Type type = In, qreal period = 0.3, qreal amplitude = 1.0,
        qreal overshoot = 1.70158f)
        : _t(type), _p(period), _a(amplitude), _o(overshoot)
    { }
    virtual ~QEasingCurveFunction() {}
    virtual qreal value(qreal t);
    virtual QEasingCurveFunction *copy() const;
    bool operator==(const QEasingCurveFunction& other);

    Type _t;
    qreal _p;
    qreal _a;
    qreal _o;
};

qreal QEasingCurveFunction::value(qreal t)
{
    return t;
}

QEasingCurveFunction *QEasingCurveFunction::copy() const
{
    return new QEasingCurveFunction(_t, _p, _a, _o);
}

bool QEasingCurveFunction::operator==(const QEasingCurveFunction& other)
{
    return _t == other._t &&
           _p == other._p &&
           _a == other._a &&
           _o == other._o;
}

#include "easing.cpp"

class QEasingCurvePrivate
{
public:
    QEasingCurvePrivate()
        : type(QEasingCurve::Linear),
          config(0),
          func(&easeNone)
    { }
    ~QEasingCurvePrivate() { delete config; }
    void setType_helper(QEasingCurve::Type);

    QEasingCurve::Type type;
    QEasingCurveFunction *config;
    QEasingCurve::EasingFunction func;
};

struct ElasticEase : public QEasingCurveFunction
{
    ElasticEase(Type type)
        : QEasingCurveFunction(type, qreal(0.3), qreal(1.0))
    { }

    QEasingCurveFunction *copy() const
    {
        ElasticEase *rv = new ElasticEase(_t);
        rv->_p = _p;
        rv->_a = _a;
        return rv;
    }

    qreal value(qreal t)
    {
        qreal p = (_p < 0) ? 0.3f : _p;
        qreal a = (_a < 0) ? 1.0f : _a;
        switch(_t) {
        case In:
            return easeInElastic(t, a, p);
        case Out:
            return easeOutElastic(t, a, p);
        case InOut:
            return easeInOutElastic(t, a, p);
        case OutIn:
            return easeOutInElastic(t, a, p);
        default:
            return t;
        }
    }
};

struct BounceEase : public QEasingCurveFunction
{
    BounceEase(Type type)
        : QEasingCurveFunction(type, 0.3f, 1.0f)
    { }

    QEasingCurveFunction *copy() const
    {
        BounceEase *rv = new BounceEase(_t);
        rv->_a = _a;
        return rv;
    }

    qreal value(qreal t)
    {
        qreal a = (_a < 0) ? 1.0f : _a;
        switch(_t) {
        case In:
            return easeInBounce(t, a);
        case Out:
            return easeOutBounce(t, a);
        case InOut:
            return easeInOutBounce(t, a);
        case OutIn:
            return easeOutInBounce(t, a);
        default:
            return t;
        }
    }
};

struct BackEase : public QEasingCurveFunction
{
    BackEase(Type type)
        : QEasingCurveFunction(type, 0.3f, 1.0f, 1.70158f)
    { }

    QEasingCurveFunction *copy() const
    {
        BackEase *rv = new BackEase(_t);
        rv->_o = _o;
        return rv;
    }

    qreal value(qreal t)
    {
        qreal o = (_o < 0) ? 1.70158f : _o;
        switch(_t) {
        case In:
            return easeInBack(t, o);
        case Out:
            return easeOutBack(t, o);
        case InOut:
            return easeInOutBack(t, o);
        case OutIn:
            return easeOutInBack(t, o);
        default:
            return t;
        }
    }
};

static QEasingCurve::EasingFunction curveToFunc(QEasingCurve::Type curve)
{
    switch(curve) {
    case QEasingCurve::Linear:
        return &easeNone;
    case QEasingCurve::InQuad:
        return &easeInQuad;
    case QEasingCurve::OutQuad:
        return &easeOutQuad;
    case QEasingCurve::InOutQuad:
        return &easeInOutQuad;
    case QEasingCurve::OutInQuad:
        return &easeOutInQuad;
    case QEasingCurve::InCubic:
        return &easeInCubic;
    case QEasingCurve::OutCubic:
        return &easeOutCubic;
    case QEasingCurve::InOutCubic:
        return &easeInOutCubic;
    case QEasingCurve::OutInCubic:
        return &easeOutInCubic;
    case QEasingCurve::InQuart:
        return &easeInQuart;
    case QEasingCurve::OutQuart:
        return &easeOutQuart;
    case QEasingCurve::InOutQuart:
        return &easeInOutQuart;
    case QEasingCurve::OutInQuart:
        return &easeOutInQuart;
    case QEasingCurve::InQuint:
        return &easeInQuint;
    case QEasingCurve::OutQuint:
        return &easeOutQuint;
    case QEasingCurve::InOutQuint:
        return &easeInOutQuint;
    case QEasingCurve::OutInQuint:
        return &easeOutInQuint;
    case QEasingCurve::InSine:
        return &easeInSine;
    case QEasingCurve::OutSine:
        return &easeOutSine;
    case QEasingCurve::InOutSine:
        return &easeInOutSine;
    case QEasingCurve::OutInSine:
        return &easeOutInSine;
    case QEasingCurve::InExpo:
        return &easeInExpo;
    case QEasingCurve::OutExpo:
        return &easeOutExpo;
    case QEasingCurve::InOutExpo:
        return &easeInOutExpo;
    case QEasingCurve::OutInExpo:
        return &easeOutInExpo;
    case QEasingCurve::InCirc:
        return &easeInCirc;
    case QEasingCurve::OutCirc:
        return &easeOutCirc;
    case QEasingCurve::InOutCirc:
        return &easeInOutCirc;
    case QEasingCurve::OutInCirc:
        return &easeOutInCirc;
    // Internal for, compatibility with QTimeLine only ??
    case QEasingCurve::InCurve:
        return &easeInCurve;
    case QEasingCurve::OutCurve:
        return &easeOutCurve;
    case QEasingCurve::SineCurve:
        return &easeSineCurve;
    case QEasingCurve::CosineCurve:
        return &easeCosineCurve;
    default:
        return 0;
    };
}

static QEasingCurveFunction *curveToFunctionObject(QEasingCurve::Type type)
{
    QEasingCurveFunction *curveFunc = 0;
    switch(type) {
    case QEasingCurve::InElastic:
        curveFunc = new ElasticEase(ElasticEase::In);
        break;
    case QEasingCurve::OutElastic:
        curveFunc = new ElasticEase(ElasticEase::Out);
        break;
    case QEasingCurve::InOutElastic:
        curveFunc = new ElasticEase(ElasticEase::InOut);
        break;
    case QEasingCurve::OutInElastic:
        curveFunc = new ElasticEase(ElasticEase::OutIn);
        break;
    case QEasingCurve::OutBounce:
        curveFunc = new BounceEase(BounceEase::Out);
        break;
    case QEasingCurve::InBounce:
        curveFunc = new BounceEase(BounceEase::In);
        break;
    case QEasingCurve::OutInBounce:
        curveFunc = new BounceEase(BounceEase::OutIn);
        break;
    case QEasingCurve::InOutBounce:
        curveFunc = new BounceEase(BounceEase::InOut);
        break;
    case QEasingCurve::InBack:
        curveFunc = new BackEase(BackEase::In);
        break;
    case QEasingCurve::OutBack:
        curveFunc = new BackEase(BackEase::Out);
        break;
    case QEasingCurve::InOutBack:
        curveFunc = new BackEase(BackEase::InOut);
        break;
    case QEasingCurve::OutInBack:
        curveFunc = new BackEase(BackEase::OutIn);
        break;
    default:
        curveFunc = new QEasingCurveFunction(QEasingCurveFunction::In, 0.3f, 1.0f, 1.70158f);     // ###
    }

    return curveFunc;
}

/*!
    Constructs an easing curve of the given \a type.
 */
QEasingCurve::QEasingCurve(Type type)
    : d_ptr(new QEasingCurvePrivate)
{
    setType(type);
}

/*!
    Construct a copy of \a other.
 */
QEasingCurve::QEasingCurve(const QEasingCurve &other)
: d_ptr(new QEasingCurvePrivate)
{
    // ### non-atomic, requires malloc on shallow copy
    *d_ptr = *other.d_ptr;
    if(other.d_ptr->config)
        d_ptr->config = other.d_ptr->config->copy();
}

/*!
    Destructor.
 */

QEasingCurve::~QEasingCurve()
{
    delete d_ptr;
}

/*!
    Copy \a other.
 */
QEasingCurve &QEasingCurve::operator=(const QEasingCurve &other)
{
    // ### non-atomic, requires malloc on shallow copy
    if (d_ptr->config) {
        delete d_ptr->config;
        d_ptr->config = 0;
    }

    *d_ptr = *other.d_ptr;
    if(other.d_ptr->config)
        d_ptr->config = other.d_ptr->config->copy();

    return *this;
}

/*!
    Compare this easing curve with \a other and returns true if they are
    equal. It will also compare the properties of a curve.
 */
bool QEasingCurve::operator==(const QEasingCurve &other) const
{
    bool res = d_ptr->func == other.d_ptr->func
            && d_ptr->type == other.d_ptr->type;
    if (res && d_ptr->config && other.d_ptr->config) {
        // catch the config content
        res = d_ptr->config->operator==(*(other.d_ptr->config));
    }
    return res;
}

/*!
    \fn bool QEasingCurve::operator!=(const QEasingCurve &other) const
    Compare this easing curve with \a other and returns true if they are not equal.
    It will also compare the properties of a curve.

    \sa operator==()
*/

/*!
    Returns the amplitude. This is not applicable for all curve types.
    It is only applicable for bounce and elastic curves (curves of type()
    QEasingCurve::InBounce, QEasingCurve::OutBounce, QEasingCurve::InOutBounce,
    QEasingCurve::OutInBounce, QEasingCurve::InElastic, QEasingCurve::OutElastic,
    QEasingCurve::InOutElastic or QEasingCurve::OutInElastic).
 */
qreal QEasingCurve::amplitude() const
{
    return d_ptr->config ? d_ptr->config->_a : 1.0;
}

/*!
    Sets the amplitude to \a amplitude.

    This will set the amplitude of the bounce or the amplitude of the
    elastic "spring" effect. The higher the number, the higher the amplitude.
    \sa amplitude()
*/
void QEasingCurve::setAmplitude(qreal amplitude)
{
    if (!d_ptr->config)
        d_ptr->config = curveToFunctionObject(d_ptr->type);
    d_ptr->config->_a = amplitude;
}

/*!
    Returns the period. This is not applicable for all curve types.
    It is only applicable if type() is QEasingCurve::InElastic, QEasingCurve::OutElastic,
    QEasingCurve::InOutElastic or QEasingCurve::OutInElastic.
 */
qreal QEasingCurve::period() const
{
    return d_ptr->config ? d_ptr->config->_p : 0.3;
}

/*!
    Sets the period to \a period.
    Setting a small period value will give a high frequency of the curve. A
    large period will give it a small frequency.

    \sa period()
*/
void QEasingCurve::setPeriod(qreal period)
{
    if (!d_ptr->config)
        d_ptr->config = curveToFunctionObject(d_ptr->type);
    d_ptr->config->_p = period;
}

/*!
    Returns the overshoot. This is not applicable for all curve types.
    It is only applicable if type() is QEasingCurve::InBack, QEasingCurve::OutBack,
    QEasingCurve::InOutBack or QEasingCurve::OutInBack.
 */
qreal QEasingCurve::overshoot() const
{
    return d_ptr->config ? d_ptr->config->_o : 1.70158f;
}

/*!
    Sets the overshoot to \a overshoot.

    0 produces no overshoot, and the default value of 1.70158 produces an overshoot of 10 percent.

    \sa overshoot()
*/
void QEasingCurve::setOvershoot(qreal overshoot)
{
    if (!d_ptr->config)
        d_ptr->config = curveToFunctionObject(d_ptr->type);
    d_ptr->config->_o = overshoot;
}

/*!
    Returns the type of the easing curve.
*/
QEasingCurve::Type QEasingCurve::type() const
{
    return d_ptr->type;
}

void QEasingCurvePrivate::setType_helper(QEasingCurve::Type newType)
{
    qreal amp = -1.0;
    qreal period = -1.0;
    qreal overshoot = -1.0;

    if (config) {
        amp = config->_a;
        period = config->_p;
        overshoot = config->_o;
        delete config;
        config = 0;
    }

    if (isConfigFunction(newType) || (amp != -1.0) || (period != -1.0) || (overshoot != -1.0)) {
        config = curveToFunctionObject(newType);
        if (amp != -1.0)
            config->_a = amp;
        if (period != -1.0)
            config->_p = period;
        if (overshoot != -1.0)
            config->_o = overshoot;
        func = 0;
    } else if (newType != QEasingCurve::Custom) {
        func = curveToFunc(newType);
    }
    assert((func == 0) == (config != 0));
    type = newType;
}

/*!
    Sets the type of the easing curve to \a type.
*/
void QEasingCurve::setType(Type type)
{
    if (d_ptr->type == type)
        return;
    if (type < Linear || type >= NCurveTypes - 1) {
        printf("QEasingCurve: Invalid curve type %d\n", type);
        return;
    }

    d_ptr->setType_helper(type);
}

/*!
    Sets a custom easing curve that is defined by the user in the function \a func.
    The signature of the function is qreal myEasingFunction(qreal progress),
    where \e progress and the return value is considered to be normalized between 0 and 1.
    (In some cases the return value can be outside that range)
    After calling this function type() will return QEasingCurve::Custom.
    \a func cannot be zero.

    \sa customType()
    \sa valueForProgress()
*/
void QEasingCurve::setCustomType(EasingFunction func)
{
    if (!func) {
        printf("Function pointer must not be null\n");
        return;
    }
    d_ptr->func = func;
    d_ptr->setType_helper(Custom);
}

/*!
    Returns the function pointer to the custom easing curve.
    If type() does not return QEasingCurve::Custom, this function
    will return 0.
*/
QEasingCurve::EasingFunction QEasingCurve::customType() const
{
    return d_ptr->type == Custom ? d_ptr->func : 0;
}

/*!
    Return the effective progress for the easing curve at \a progress.
    While  \a progress must be between 0 and 1, the returned effective progress
    can be outside those bounds. For instance, QEasingCurve::InBack will
    return negative values in the beginning of the function.
 */
qreal QEasingCurve::valueForProgress(qreal progress) const
{
    progress = qBound<qreal>(0, progress, 1);
    if (d_ptr->func)
        return d_ptr->func(progress);
    else if (d_ptr->config)
        return d_ptr->config->value(progress);
    else
        return progress;
}
