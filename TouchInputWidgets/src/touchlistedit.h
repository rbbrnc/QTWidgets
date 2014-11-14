#ifndef TOUCHLISTEDIT_H
#define TOUCHLISTEDIT_H

#include <QLineEdit>

class TouchListEdit : public QLineEdit
{
    Q_OBJECT
    public:
        explicit TouchListEdit(QWidget *parent = 0);
        void setList(const QStringList &list);

    protected:
        void mousePressEvent(QMouseEvent *event);

    signals:
        void clicked();

	private:
		QStringList m_values;

};

#endif
