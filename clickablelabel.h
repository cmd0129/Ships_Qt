#ifndef CLICKABLELABEL_H
#define CLICKABLELABEL_H

#include <QLabel>
#include <QWidget>
#include <Qt>

/*! \file clickablelabel.h
 *   \brief Plik nagłówkowy zawierający klasę ClickableLabel.
 */

/*!
 * \brief Klasa ClickableLabel odpowiada za wywoływanie zdarzeń po naciśnięciu na pole.
 *
 *
 */
class ClickableLabel : public QLabel {
    Q_OBJECT

public:
    /*!
     * \brief Konstruktor klasy ClickableLabel.
     *
     * \param parent odpowiada za rodzica obiektu.
     * \param f flaga.
     */
    explicit ClickableLabel(QWidget* parent = Q_NULLPTR, Qt::WindowFlags f = Qt::WindowFlags());

    /*!
     * \brief Destruktor klasy ClickableLabel.
     */
    ~ClickableLabel();

signals:
    /*!
     * \brief Sygnał odpowiadający za wciśnięcie.
     */
    void clicked();

protected:
    /*!
     * \brief Metoda odpowiadająca za emisję sygnału clicked()
     *
     * \param event naciśnięcie.
     */
    void mousePressEvent(QMouseEvent* event);
};

#endif // CLICKABLELABEL_H
